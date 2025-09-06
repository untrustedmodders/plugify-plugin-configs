#pragma once
#include <memory>
#include <plg/plugin.hpp>
#include <string_view>

namespace pcf {
	class Config;
	class ReaderFactory;

	class ConfigsPlugin final : public plg::IPluginEntry {
	public:
		// IPluginEntry interface
		void OnPluginStart() final;
		void OnPluginEnd() final;

	public:
		std::unique_ptr<Config> ReadConfig(std::string_view path);
		std::unique_ptr<Config> ReadConfigs(std::span<const std::string_view> paths);
		void SetError(std::string_view error);
		std::string_view GetError();

	private:
		std::unique_ptr<ReaderFactory> _factory;
		std::string _error;
	};
}// namespace pcf
