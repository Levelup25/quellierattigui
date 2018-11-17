#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include "../../shared/state/ElementType.h"
#include "Element.h"
#include "Rectangle.h"
#include "Sprite.h"
#include "SpriteGeneratorById.h"
#include "TileSprite.h"
#include "Window.h"
#include "WindowManager.h"

using namespace render;
using namespace state;
using namespace std;

void testRender();
void createWindowWith(Element* root);
Element* buildRootWindow();
Element* buildRootTestRectangle();
Element* buildRootWebpageStyle();
Element* buildRootSprite();
Element* testCopy();
