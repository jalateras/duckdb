#include "duckdb/parser/parsed_data/sample_options.hpp"
#include "duckdb/common/field_writer.hpp"
#include "duckdb/common/serializer/enum_serializer.hpp"
#include "duckdb/common/serializer/format_serializer.hpp"
#include "duckdb/common/serializer/format_deserializer.hpp"

namespace duckdb {

string SampleMethodToString(SampleMethod method) {
	return EnumSerializer::EnumToString(method);
}

void SampleOptions::Serialize(Serializer &serializer) {
	FieldWriter writer(serializer);
	writer.WriteSerializable(sample_size);
	writer.WriteField<bool>(is_percentage);
	writer.WriteField<SampleMethod>(method);
	writer.WriteField<int64_t>(seed);
	writer.Finalize();
}

void SampleOptions::FormatSerialize(FormatSerializer &serializer) const {
	serializer.WriteProperty("sample_size", sample_size);
	serializer.WriteProperty("is_percentage", is_percentage);
	serializer.WriteProperty("method", method);
	serializer.WriteProperty("seed", seed);
}

std::unique_ptr<SampleOptions> SampleOptions::FormatDeserialize(FormatDeserializer &deserializer) {
	auto result = make_unique<SampleOptions>();

	deserializer.ReadProperty("sample_size", result->sample_size);
	deserializer.ReadProperty("is_percentage", result->is_percentage);
	deserializer.ReadProperty("method", result->method);
	deserializer.ReadProperty("seed", result->seed);

	return result;
}

unique_ptr<SampleOptions> SampleOptions::Deserialize(Deserializer &source) {
	auto result = make_unique<SampleOptions>();

	FieldReader reader(source);
	result->sample_size = reader.ReadRequiredSerializable<Value, Value>();
	result->is_percentage = reader.ReadRequired<bool>();
	result->method = reader.ReadRequired<SampleMethod>();
	result->seed = reader.ReadRequired<int64_t>();
	reader.Finalize();

	return result;
}

unique_ptr<SampleOptions> SampleOptions::Copy() {
	auto result = make_unique<SampleOptions>();
	result->sample_size = sample_size;
	result->is_percentage = is_percentage;
	result->method = method;
	result->seed = seed;
	return result;
}

bool SampleOptions::Equals(SampleOptions *a, SampleOptions *b) {
	if (a == b) {
		return true;
	}
	if (!a || !b) {
		return false;
	}
	if (a->sample_size != b->sample_size || a->is_percentage != b->is_percentage || a->method != b->method ||
	    a->seed != b->seed) {
		return false;
	}
	return true;
}

} // namespace duckdb
