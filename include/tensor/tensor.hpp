#pragma once
#include <string>
#include <cstddef>

inline constexpr int GAP_DIMS = 2;

namespace ace
{
	/* Tensor: lightweight tensor handle (view by default).
		- No internal allocation in hot paths.
		- Backend attaches an externally-allocated buffer via attach().
		- If a deleter is provided, Tensor becomes owning and will call it on destruction. */

	template <typename T>
	class Tensor
	{
	public:
		static_assert(GAP_DIMS >= 2, "Tensor expects GAP_DIMS >= 2 for rows()/cols().");

		// Non-owning 2D tensor view. Use attach() to adopt a backend-owned buffer.
		Tensor(const std::string &name, size_t rows, size_t cols);

		// name & type
		const std::string &name() const noexcept { return name_; }
		static std::string typeName();

		// shape & stride
		const size_t *shape() const noexcept { return ne_; }
		const size_t *stride() const noexcept { return nb_; }
		size_t rows() const noexcept { return ne_[0]; }
		size_t cols() const noexcept { return ne_[1]; }

		// data access
		T *data() noexcept { return static_cast<T *>(data_); }
		const T *data() const noexcept { return static_cast<const T *>(data_); }
		void *raw() noexcept { return data_; }
		const void *raw() const noexcept { return data_; }

		// ownership / backend integration
		using Deleter = void (*)(void* ptr, void* ctx) noexcept;

		// Attach external buffer. If del != nullptr, Tensor becomes owning and will
		// call del(ptr, ctx) on destruction or on next attach()/reset().
		// REQUIRE: ptr is aligned to at least alignof(T) and backend policy.
		void attach(void *ptr, Deleter del = nullptr, void *ctx = nullptr) noexcept;

		// Release buffer and its ownership (if any) to caller.
		// Returns the raw pointer; no deleter is invoked.
		[[nodiscard]] void *release() noexcept;

		// Clear buffer (invokes deleter if owning).
		void reset() noexcept { dispose(); }

		bool isOwning() const noexcept { return owning_; }
		static constexpr size_t alignment() noexcept { return alignof(T); }
		static constexpr size_t elementSize() noexcept { return sizeof(T); }

		// move
		Tensor(Tensor &&) noexcept;
		Tensor &operator=(Tensor &&) noexcept;

		// copy disabled
		Tensor(const Tensor &) = delete;
		Tensor &operator=(const Tensor &) = delete;

		~Tensor() { dispose(); };

	private:
		std::string name_;
		size_t ne_[GAP_DIMS]; // shape
		size_t nb_[GAP_DIMS]; // stride

		void *data_ = nullptr; // data pointer
		Deleter deleter_ = nullptr;
		void* deleter_ctx_ = nullptr;
		bool owning_ = false;

		void dispose() noexcept; // invoke deleter if owning	
	};

} // namespace gap

#include "tensor.tpp" // 구현부 포함
