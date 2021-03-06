// Generated by dia2code
#ifndef RENDER__RELATIF__H
#define RENDER__RELATIF__H

#include <string>
#include <ostream>

namespace render {
  class Relatif;
}

#include "ComputeMethodType.h"

namespace render {

  /// class Relatif - 
  class Relatif {
    // Associations
    // Attributes
  private:
    ComputeMethodType computeMethod     = ComputeMethodType::pixel;
    float pixel     = 0;
    float percent     = 0;
    std::string alignement;
    // Operations
  public:
    Relatif ();
    Relatif (int pixel);
    Relatif (float pixel);
    Relatif (std::string str);
    void operator= (int pixel);
    void operator= (float pixel);
    void operator= (const char* c);
    void operator= (std::string str);
    ComputeMethodType getComputeMethod () const;
    float getPixel () const;
    float getPercent () const;
    std::string getAlignement () const;
    friend std::ostream& operator<< (std::ostream& os, const Relatif& rel);
    Relatif (const char* c);
    // Setters and Getters
  };

};

#endif
