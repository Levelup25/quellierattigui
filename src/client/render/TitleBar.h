// Generated by dia2code
#ifndef RENDER__TITLEBAR__H
#define RENDER__TITLEBAR__H

#include <string>

namespace render {
  class TitleBarButton;
  class Rectangle;
}

#include "Rectangle.h"
#include "TitleBarButton.h"

namespace render {

  /// class TitleBar - 
  class TitleBar : public render::Rectangle {
    // Associations
    // Attributes
  public:
    TitleBarButton* pcloseBtn;
  protected:
    std::string title;
    // Operations
  public:
    TitleBar ();
    // Setters and Getters
    const std::string& getTitle() const;
    void setTitle(const std::string& title);
  };

};

#endif