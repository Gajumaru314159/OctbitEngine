﻿//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/File/INI.h>
#include <Framework/Core/File/FileStream.h>
#include <fstream>
#include <sstream>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  キーを指定して要素アクセス
	//@―---------------------------------------------------------------------------
	String& INISection::operator[](String key) {
		return m_container[key];
	}

	//@―---------------------------------------------------------------------------
	//! @brief  キーとフォールバックを指定して要素を取得
	//@―---------------------------------------------------------------------------
	String INISection::get(String key, String fallback)const {
		if (m_container.count(key))return m_container.find(key)->second;
		return fallback;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  対象のキーが存在するか
	//@―---------------------------------------------------------------------------
	bool INISection::has(String key)const {
		return  m_container.count(key);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  対象のキーを削除
	//@―---------------------------------------------------------------------------
	bool INISection::remove(String key) {
		return m_container.erase(key);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  要素をすべて削除
	//@―---------------------------------------------------------------------------
	void INISection::clear() {
		m_container.clear();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  要素数
	//@―---------------------------------------------------------------------------
	s32 INISection::size()const {
		return m_container.size();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	INI::INI() {

	}

	//@―---------------------------------------------------------------------------
	//! @brief  ファイルから設定をロードして生成
	//@―---------------------------------------------------------------------------
	INI::INI(String path) {
		m_path = path;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ~デストラクタ
	//@―---------------------------------------------------------------------------
	INI::~INI() {

	}

	//@―---------------------------------------------------------------------------
	//! @brief  ファイルから設定をロード
	//@―---------------------------------------------------------------------------
	bool INI::load(String path) {
		m_path = path;

		std::basic_ifstream<Char> ifs(m_path.c_str());
		std::basic_stringstream<Char> ss;
		ss << ifs.rdbuf();

		std::basic_string<Char> buf;
		String section;


		while (std::getline(ss, buf)) {

			String str(buf);
			str.trim();

			// 空行
			if (str.empty())continue;

			// コメント
			if (str.starts_with(TC(';')) || str.starts_with(TC('#'))) continue;

			// セクション
			if (str.starts_with(TC('[')) && str.ends_with(TC(']'))) {
				section = str.substr(1, str.size() - 2);
				continue;
			}

			auto equalPos = str.find(TC('='));

			if (equalPos == str.npos)continue;

			String key{ str.substr(0, equalPos).trim() };
			String value{ str.substr(equalPos + 1).trim() };

			(*this)[section][key] = value;

		}

		return true;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ファイルに設定をセーブ
	//! @details ファイル名が未設定の場合、ロード時のパスに上書き保存されます。
	//@―---------------------------------------------------------------------------
	bool INI::save(String path) {

		if (path.empty()) path = m_path;
		if (path.empty()) return false;

		FileStream fs(path, FileOpenMode::Write);
		if (!fs) return false;

		auto str = string();

		return fs.write(str.data(), str.size());

	}

	//@―---------------------------------------------------------------------------
	//! @brief  文字列として出力
	//@―---------------------------------------------------------------------------
	String INI::string() {
		std::stringstream ss;
		for (auto& [section, scon] : m_container) {

			ss << Format("[{}]\n", section);

			for (auto& [key, value] : scon) {

				ss << Format("{} = {}\n", key, value);

			}

		}
		return ss.str();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  キーを指定して要素アクセス
	//@―---------------------------------------------------------------------------
	INISection& INI::operator[](String key) {
		return m_container[key];
	}

	//@―---------------------------------------------------------------------------
	//! @brief  キーとフォールバックを指定して要素を取得
	//@―---------------------------------------------------------------------------
	String INI::get(String section, String key, String fallback)const {
		if (has(section)) {
			m_container.at(section).get(key, fallback);
		}
		return fallback;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  対象のセクションが存在するか
	//@―---------------------------------------------------------------------------
	bool INI::has(String section)const {
		return m_container.count(section);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  対象のキーが存在するか
	//@―---------------------------------------------------------------------------
	bool INI::has(String section, String key)const {
		return has(section) && m_container.at(section).has(key);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  対象のセクションを削除
	//@―---------------------------------------------------------------------------
	void INI::remove(String section) {
		m_container.erase(section);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  対象のキーを削除
	//@―---------------------------------------------------------------------------
	void INI::remove(String section, String key) {
		if (has(section)) {
			m_container.at(section).remove(key);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  要素をすべて削除
	//@―---------------------------------------------------------------------------
	void INI::clear() {
		return m_container.clear();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  要素数
	//@―---------------------------------------------------------------------------
	s32 INI::size()const {
		return m_container.size();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  要素が空か
	//@―---------------------------------------------------------------------------
	bool INI::empty()const {
		return m_container.empty();
	}

}