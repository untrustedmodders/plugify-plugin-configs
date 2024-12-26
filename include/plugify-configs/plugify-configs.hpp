#pragma once
#include "methods.hpp"
#include <plugify_configs_export.h>
#include <memory>

namespace pcf
{
	class Config
	{
	public:
		enum class NodeType
		{
			Null,
			Bool,
			Int,
			Float,
			String,
			Object,
			Array,
		};

	public:
		PLUGIFY_CONFIGS_API virtual ~Config();

		PLUGIFY_CONFIGS_API void Set(int64_t value);

	public:
		class Detail;

	private:
		friend Config::Detail;

		Config() = delete;
		Config(std::unique_ptr<Detail> detail);

	private:
		std::unique_ptr<Detail> _detail;
	};
}
