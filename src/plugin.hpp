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
		std::unique_ptr<Config> ReadConfigs(const plg::vector<std::string_view>& paths);
		void SetError(std::string_view error);
		plg::string GetError();

	private:
		std::unique_ptr<ReaderFactory> _factory;
		plg::string _error;
	};
}
