#pragma once
#include <plugify/cpp_plugin.hpp>
#include <string_view>
#include <memory>

namespace pcf
{
	class Config;
	class ReaderFactory;

	class ConfigsPlugin final : public plg::IPluginEntry {
	public:
		// IPluginEntry interface
		void OnPluginStart() final;
		void OnPluginEnd() final;

	public:
		std::unique_ptr<Config> ReadConfig(std::string_view path);

	private:
		std::unique_ptr<ReaderFactory> _factory;
	};
}
