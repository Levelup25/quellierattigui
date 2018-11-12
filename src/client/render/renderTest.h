#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include "Element.h"
#include "Rectangle.h"
#include "Window.h"
#include "WindowManager.h"

using namespace render;
using namespace std;

void testRender();
void createWindowWith(Element* root);
Element* buildRootWindow();
Element* buildRootTestRectangle();
Element* buildRootWebpageStyle();
Element* buildRootSprite();
