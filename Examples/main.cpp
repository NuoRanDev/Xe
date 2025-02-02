#include "XeCore.hpp"
#include "xeAssets.hpp"

int main(int argc, char* argv[])
{
	xe::Application::LaodApplication(argc, argv, "test.log");
	system("chcp 65001");
	xe::ioMmapfstream file = xe::ioMmapfstream();
	xe::xeString str1 = "一クロノクロック€é+×№<₽₸₥₭₭₼éêêëËËËœœɵÛŨü↚↮↩↢◀▧▪⌍⌟⅔⅐⅕⅗ⅧⅨⅥⅶ∵∲∶⊣⊠⊴⊸⊺⊹⊩②⒎⒔⨅⨈⨋⨊⨌⨿⨹⨺⩀⩃⩑⩏⩑〰〆〓㈠⺔⻉ㄡㄠ无修屌日天";
	xe::xeString str2 = "₽₸₥₭₭₼éêêëËËËœœɵÛŨü↚↮↩↢◀▧▪⌍⌟⅔⅐⅕⅗ⅧⅨⅥⅶ∵∲∶";
	str1.Append(str2);
	str1.Append(U'屌');
	std::cout << str1 << std::endl;
	char32_t diao = str1[4];
	xe::xeString str3 = str1.Slice(2, 5);
	std::cout << str3 << std::endl;
	return xe::Application::DestroyApplication();
}