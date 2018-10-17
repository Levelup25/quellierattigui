#include "TabElement.h"

namespace state {

    TabElement::TabElement (std::size_t width, std::size_t height) {
        for(std::size_t i=0;i<height;i++) {
                list.push_back(std::move(std::unique_ptr<Element>(new Element())));
        }
    }
    std::size_t const TabElement::getWidth () {
        return this->width;
    }
    std::size_t const TabElement::getHeight () {
        return this->height;
    }
    void TabElement::add (Element* e) {
        list.push_back(std::move(std::unique_ptr<Element>(e)));
        this->height++;
    }
    //void resize (std::size_t width, std::size_t height);
    Element* const TabElement::get (int i, int j) {
        return list[i].get();
    }
    void TabElement::set (Element* e, int i, int j) {
        list[i]=(std::move(std::unique_ptr<Element>(e)));
    }
    void TabElement::del (int i, int j) {
        list.erase(list.begin()+i);
        this->height--;
    }

}
