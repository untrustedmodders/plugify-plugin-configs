#pragma once
#include <plugify/cpp_plugin.hpp>

namespace pcf
{
	class ConfigsPlugin final : public plg::IPluginEntry {
	public:
		// IPluginEntry interface
		void OnPluginStart() final;
		void OnPluginEnd() final;
	};
}
