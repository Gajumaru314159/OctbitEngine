//***********************************************************
//! @file
//! @brief		ワールド
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Editor.h>

namespace ob::engine {

	void Editor::send(const Notice& notice) {

		struct NoticeHeader {
			u64 size;
			u64 type;
		};

		Blob blob;
		BinaryStream stream(blob);
		BinaryWriter writer(stream);

		writer.writeUInt64(0); // サイズを書き込むためのダミー
		writer.writeUInt64(Hash::FNV64(notice.getType().shortName()));
		notice.serialize(writer);
		writer.seek(0);
		writer.writeUInt64(blob.size() - sizeof(NoticeHeader));

		LOG_INFO("{}を送信", notice.getType().shortName());

		send(blob);
	}

	void Editor::flush() {
		ScopeLock lock(m_sendBufferLock);
		if (m_sendBuffer.empty())return;
		m_client.send((char*)m_sendBuffer.data(), m_sendBuffer.size());
		m_sendBuffer.clear();
	}

	void Editor::receive() {

		// 受信
		while (true) {
			char buffer[1024];
			auto size = m_client.receive(buffer, std::size(buffer));
			m_blob.append(buffer, size);
			if (size != std::size(buffer))break;
		}

		BinaryStream stream(m_blob);
		BinaryReader reader(stream);

		while (true) {

			const auto position = stream.position();
			const auto remain = stream.size() - position;

			struct QueryHeader {
				u64				size;
				u64				type;
				u64				id;
			} header;

			header.size = reader.readU64();
			header.type = reader.readU64();
			header.id   = reader.readU64();

			// バッファサイズが足りなければシークを戻して終了
			if (remain < sizeof(QueryHeader) + header.size) {
				stream.seek(position);
				break;
			}

			const auto end = position + sizeof(QueryHeader) + header.size;

			// デシリアライズ
			auto itr = m_queries.find(header.type);
			if (itr != m_queries.end()) {

				auto& query = *itr->second;

				LOG_INFO("{}を受信", query.getType().shortName());

				if (query.deserialize(reader)) {
					if (auto response = std::unique_ptr<Response>(query.execute())) {

						LOG_INFO("{}を返信", response->getType().shortName());

						m_writeBlob.clear();
						BinaryStream writerStream(m_writeBlob);
						BinaryWriter writer(writerStream);

						struct ResponseHeader {
							u64 size;
							u64 type;
							u64 id;
						};

						writer.writeUInt64(0); // サイズを書き込むためのダミー
						writer.writeUInt64(Hash::FNV64(response->getType().shortName()));
						writer.writeUInt64(header.id);
						response->serialize(writer);
						writer.seek(0);
						writer.writeUInt64(m_writeBlob.size() - sizeof(ResponseHeader));

						send(m_writeBlob);

					} else {
						// LOG_ERROR("レスポンスが空です");
					}
				}
			}

			// クエリの読み取りで早期リターンしても問題ないように明示的に最後へシーク
			stream.seek(end);

		}

		// ストリームで読み込み済みの分をずらす
		if (0 < stream.position()) {
			auto remain = stream.size() - stream.position();
			memmove_s(m_blob.data(), m_blob.size(), m_blob.data() + stream.position(), remain);
			m_blob.resize(remain);
		}

	}

}
