#include<iostream>
#include<variant>
#include<vector>

#define VARIANT_TYPES(...) std::variant<__VA_ARGS__>
#define X_(T) std::unique_ptr<T>
#define LIST std::vector<AllType>


/// <summary>
/// 需要按照该格式：X_(type),向数组里面存放你需要的存放的类型
/// </summary>
using AllType = VARIANT_TYPES(X_(int), X_(char), X_(double));


/// <summary>
/// 添加向list元素,按照append<value_type>(list,value);格式
/// </summary>
/// <typeparam name="T"></typeparam>
/// <typeparam name="...Args"></typeparam>
/// <param name="list"></param>
/// <param name="...args"></param>
template<typename T, typename... Args>
void append(LIST& list, Args&&... args) {
	(void)std::initializer_list<int>{
		(list.emplace_back(std::make_unique<T>(std::forward<Args>(args))), 0)...
	};
}


/// <summary>
/// 查找list里的所有元素
/// </summary>
/// <param name="list"></param>
void show(LIST& list) {
	for (const auto& item : list) {
		std::visit([](const auto& ptr) {
			if (ptr) {
				std::cout << *ptr << "\t";
			}
			}, item);
	}
	std::cout << std::endl;
}

/// <summary>
/// 查看list里的特定type的所有元素
/// 格式：show<search_type>(list)
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="list"></param>
template<typename T>
void find(LIST& list) {
	for (const auto& item : list) {
		std::visit([&](const auto& ptr) {
			using PtrType = std::decay_t<decltype(ptr)>;
			if constexpr (std::is_same_v<PtrType, std::unique_ptr<T>>) {
				if (ptr) { 
					std::cout << *ptr << "\t";
				}
			}
			}, item);
	}
	std::cout << std::endl;
}

/// <summary>
/// 例子
/// </summary>
void example() {
	LIST list1;
	append<double>(list1, 1.5, 8.25);
	append<int>(list1, 500, 7500, 6);
	append<char>(list1, 'c', 'k');
	show(list1);
	find<char>(list1);
	find<int>(list1);
	find<double>(list1);
}
int main() {
	example();
	return 0;
}