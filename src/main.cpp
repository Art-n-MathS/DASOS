#include <QApplication>
#include "MainWindow.h"


int main(int argc, char **argv)
{

  QApplication a(argc, argv);

  MainWindow w;
  std::cout << "~Main Window created\n";
  // show it
  w.show();

  std::cout << "   ***   EXIT   ***\n";
  return a.exec();
}



//#include <iostream>
//#include <unordered_map>

//int main(int argc, char **argv)
//{
//   // define and initialise map
//   std::unordered_multimap < unsigned int , unsigned int > myMap;

//   // add more elements
//   myMap.emplace(100,500);

//   for (unsigned int i=0; i< 60; ++i)
//   {
//      myMap.emplace(100,i);
//   }
//   myMap.emplace(200,20);

//   //loop through all its elements
//   std::cout << "These are all the elements saved into the map:\n";
//   for (auto& x: myMap)
//   {
//      std::cout << "(" << x.first << " , " << x.second << ")\n";
//   }

//   // print all the elements with Key = 100
//   unsigned int key = 100;
//   std::cout << "These are all the elements with key value: " << key << "\n";
//   auto itsElements = myMap.equal_range(key);
//   for (auto it = itsElements.first; it != itsElements.second; ++it)
//   {
//       std::cout << "(" << it->first << " , " << it->second << ")\n";
//   }
//   return 0;
//}
