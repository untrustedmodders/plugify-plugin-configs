#include "readerfactory.hpp"
#include <filesystem>

#include <plugify-configs/plugify-configs.hpp>

namespace pcf {
	void ReaderFactory::RegisterReader(std::string_view ext, ReaderFactory::ReaderLoad func) {
		if (func == nullptr) {
			return;
		}

		if (_readers.contains(ext)) {
			SetError(std::format("Reader for {} already registered", ext));
			return;
		}

		_readers.emplace(ext, func);
	}

	std::unique_ptr<Config> ReaderFactory::ReadConfig(std::string_view path) {
		namespace fs = std::filesystem;
		const auto config_path = fs::path(path);
		const auto ext = config_path.extension().string();
		if (ext.empty()) {
			SetError("Path has no extension");
			return nullptr;
		}
		const auto reader = _readers.find(std::string_view(ext).substr(1));
		if (reader == _readers.end()) {
			SetError(std::format("No factory for {}", ext));
			return nullptr;
		}
		return (reader->second)(path);
	}
}// namespace pcf
