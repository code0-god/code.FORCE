#pragma once
#include <string>

namespace gap
{
	template <typename T>
	class Tensor
	{
	public:
		// 타입 별칭 (표준 컨테이너와 일관되게)
		using value_type = T;
		using pointer = T *;
		using const_pointer = const T *;
		using reference = T &;
		using const_reference = const T &;

		Tensor(const std::string &name, int rows, int cols);
		const std::string &name() const noexcept { return name_; };
		int rows() const noexcept { return ne_[0]; }
		int cols() const noexcept { return ne_[1]; }

		static std::string typeName();
		~Tensor();

	private:
		std::string name_;
		int ne_[2]; // shape
		int nb_[2]; // stride
		void *data_ = nullptr;
	};

} // namespace gap

#include "tensor.tpp" // 구현부 포함
