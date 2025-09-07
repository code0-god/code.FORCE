#pragma once

template <typename T> 
class tensor {
	public:
		tensor(){};
		
	private:
		int ne[2];
		int nb[2];
		void* data_ = nullptr;

		
};
