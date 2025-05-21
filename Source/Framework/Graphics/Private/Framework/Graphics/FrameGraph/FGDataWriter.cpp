//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/FrameGraph/FGDataWriter.h>

namespace ob::graphics {


	FGDataWriter::FGDataWriter(FGData& data)
		: m_data(data)
	{

	}

	void FGDataWriter::operator()(const PassNode& node, const std::vector<ResourceNode>& resourceNodes) {
		m_data.passes.push_back(
			FGData::Pass{
				node.getId(),
				node.getName().data(),
				!node.canExecute()
			}
		);
		const auto getResourceIds = [&](const auto type) {
			Vector<u32> ids;
			ids.reserve(resourceNodes.size());
			for (const auto [id, _] : node.each(type)) {
				ids.emplace_back(resourceNodes[id].getResourceId());
			}
			return ids;
			};
		m_data.reads = getResourceIds(PassNode::Read{});
		m_data.writes = getResourceIds(PassNode::Write{});
	}
	void FGDataWriter::operator()(const ResourceNode& node, const ResourceEntry& entry, const std::vector<PassNode>& passNodes) {

		auto [itr, added] = m_data.resources.try_emplace(entry.getId());
		auto& obj = itr->second;

		if (added) {
			obj = {
				entry.getId(),
				String(node.getName().data()),
				entry.toString(),
				entry.isTransient()
			};
		}
		for (const auto& p : passNodes) {
			if (p.creates(node.getId())) {
				obj.createdPassId = p.getId();
			}
			if (p.reads(node.getId())) {
				obj.readers.emplace_back(p.getId());
			}
			if (p.writes(node.getId())) {
				obj.writers.emplace_back(p.getId());
			}
		}
	}
	void FGDataWriter::flush(std::ostream& os) const {}

}