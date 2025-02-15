#include "readerfactory.hpp"
#include <plugify-configs/plugify-configs.hpp>
#include <filesystem>

namespace pcf
{
	void ReaderFactory::RegisterReader(std::string_view ext, ReaderFactory::ReaderLoad func)
	{
		if (func == nullptr) {
			return;
		}

		_readers[ext] = func;
	}

	std::unique_ptr<Config> ReaderFactory::ReadConfig(std::string_view path)
	{
		namespace fs = std::filesystem;
		const auto config_path = fs::path(path);
		const auto ext = config_path.extension().string();
		const auto reader = _readers.find(ext);
		if (reader != _readers.end()) {
			return (reader->second)(path);
		}
		SetError(std::format("No factory for {}", ext));
		return nullptr;
	}
}
