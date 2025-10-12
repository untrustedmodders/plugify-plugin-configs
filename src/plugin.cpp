#include "plugin.hpp"
#include "readerfactory.hpp"
#include "readers/json_glaze.hpp"

#include <plugify-configs/plugify-configs.hpp>
#include <plugify_configs_export.h>

namespace pcf {
	void ConfigsPlugin::OnPluginStart() {
		_factory = std::make_unique<ReaderFactory>();
		_factory->RegisterReader("json", &ReadJsonConfig);
		_factory->RegisterReader("jsonc", &ReadJsoncConfig);
	}

	void ConfigsPlugin::OnPluginEnd() {
		_factory.reset();
	}

	std::unique_ptr<Config> ConfigsPlugin::ReadConfig(std::string_view path) {
		_error.clear();

		if (!_factory) {
			_error = "Factory is null";
			return nullptr;
		}

		auto config = _factory->ReadConfig(path);

		if (!config && _error.empty()) {
			// TODO: Log error: No error is setted while config parse failure
		}

		return config;
	}

	std::unique_ptr<Config> ConfigsPlugin::ReadConfigs(std::span<const std::string_view> paths) {
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

		size_t merged = 0;

		for (const std::string_view& path: paths) {
			namespace fs = std::filesystem;
			std::error_code ec;
			if (fs::exists(path, ec)) {
				auto overrideConfig = _factory->ReadConfig(path);
				if (!overrideConfig) {
					if (_error.empty()) {
						// TODO: Log error: No error is setted while config parse failure
					}
					return nullptr;
				}
				config->MergeMove(std::move(*overrideConfig));
				++merged;
			}
		}

		if (merged == 0) {
			_error = "Failed to find any config at paths";
			return nullptr;
		}

		return config;
	}

	void ConfigsPlugin::SetError(std::string_view error) {
		_error = error;
	}

	std::string_view ConfigsPlugin::GetError() {
		return _error;
	}

	ConfigsPlugin plugin;
}// namespace pcf

EXPOSE_PLUGIN(PLUGIFY_CONFIGS_API, pcf::ConfigsPlugin, &pcf::plugin)
