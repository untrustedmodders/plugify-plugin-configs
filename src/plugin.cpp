#include "readers/json_glaze.hpp"
#include "plugin.hpp"
#include "readerfactory.hpp"
#include <plugify-configs/plugify-configs.hpp>
#include <plugify_configs_export.h>

namespace pcf
{
	void ConfigsPlugin::OnPluginStart()
	{
		_factory = std::make_unique<ReaderFactory>();
		_factory->RegisterReader("json", &ReadJsonConfig);
	}

	void ConfigsPlugin::OnPluginEnd()
	{
		_factory.reset();
	}

	std::unique_ptr<Config> ConfigsPlugin::ReadConfig(std::string_view path)
	{
		_error.clear();

		if (!_factory) {
			_error = "Factory is null";
			return nullptr;
		}

		auto config = _factory->ReadConfig(path);

		if (!config && _error.length() == 0) {
			// TODO: Log error: No error is setted while config parse failure
		}

		return config;
	}

	std::unique_ptr<Config> ConfigsPlugin::ReadConfigs(const plg::vector<std::string_view>& paths)
	{
		_error.clear();

		if (!_factory) {
			_error = "Factory is null";
			return nullptr;
		}

		if (paths.empty()) {
			_error = "Empty paths array";
			return nullptr;
		}

		auto config = MakeConfig();
		if (!config) {
			_error = "Failed to make config";
			return nullptr;
		}

		for (std::string_view path : paths) {
			namespace fs = std::filesystem;
			if (fs::exists(path)) {
				auto overrideConfig = _factory->ReadConfig(path);
				if (!overrideConfig) {
					if (_error.length() == 0) {
						// TODO: Log error: No error is setted while config parse failure
					}
					return nullptr;
				}
				config->MergeMove(std::move(*overrideConfig));
			}
		}

		return config;
	}

	void ConfigsPlugin::SetError(std::string_view error)
	{
		_error = error;
	}

	plg::string ConfigsPlugin::GetError()
	{
		return _error;
	}

	ConfigsPlugin plugin;
}

EXPOSE_PLUGIN(PLUGIFY_CONFIGS_API, &pcf::plugin)
