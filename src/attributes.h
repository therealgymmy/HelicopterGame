#ifdef INCL_ATTRIBUTES

#define self crtp_cast<D>(*this)

namespace Attributes {

// Shape
class Ellipse   {};
class Rectangle {};

// Property
class Border {};
class Fill   {};

// Color
enum Color {
    BLACK,
    WHITE,
    GREY,
    GREEN,
    BROWN,
    RED,
};

// Type
enum Static {
    IsMember,
    IsStatic,
};

// <Derived, Shape, Property>
template <typename D, typename S, typename P>
class Drawable {};

template <typename D, Static S>
class XYCoordinates {};

template <typename D, Static S>
class WHSizes {};

template <typename D, Static S>
class Speed {};

}

namespace attr = Attributes;

#undef self

#endif

// Hacky way to include attribute implementations without multiple definitions
#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H
 #include "attributes_impl.h"
#endif
