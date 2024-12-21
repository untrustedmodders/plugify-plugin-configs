#pragma once
#include <plugify/cpp_plugin.hpp>
#include <memory>

namespace pcf
{
	class ReaderFactory;

	class ConfigsPlugin final : public plg::IPluginEntry {
	public:
		// IPluginEntry interface
		void OnPluginStart() final;
		void OnPluginEnd() final;

	private:
		std::unique_ptr<ReaderFactory> _factory;
	};
}
