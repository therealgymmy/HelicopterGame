/*
 * Implementation Header File
 * Do NOT include this file anywhere other than attributes.h
 */

#ifndef ATTRIBUTES_IMPL_H
#define ATTRIBUTES_IMPL_H

#include "error.h"
#include "crtp.h"
#include "x_resources.h"

// Hacky way to refer back to attributes.h without cyclic dependency
#define INCL_ATTRIBUTES
 #include "attributes.h"
#undef  INCL_ATTRIBUTES

#define self crtp_cast<D>(*this)

/* Attribute Implementations */

// (name, width, height, shape, property)
/*
#define INTERNAL_SHAPE_DECL(n, w, h, s, p) \
    internal_shape__##n##_ : public attr::WHSizes<internal_shape__##n##_,\
                                                  attr::IsStatic>, \
                             public attr::Drawable<internal_shape__##n##_, \
                                                     attr::s, \
                                                     attr::p>

#define INTERNAL_SHAPE_INIT(n, w, h, s, p) \
    friend class attr::Drawable<internal_shape__##n##_, attr::s, attr::p>; \
    friend class attr::WHSizes<internal_shape__##n##_, attr::IsStatic>; \
    public: \
        typedef attr::Drawable<internal_shape__##n##_, attr::s, attr::p> Draw; \
        typedef attr::WHSizes<internal_shape__##n##_, attr::IsStatic> Size; \
        static const int w_ = w; \
        static const int h_ = h;
*/
#define INTERNAL_SHAPE_DECL(n, w, h, s, p) \
    internal_shape__##n##_ : public attr::Drawable<internal_shape__##n##_, \
                                                   attr::s, \
                                                   attr::p>

#define INTERNAL_SHAPE_INIT(n, w_, h_, s, p) \
    friend class attr::Drawable<internal_shape__##n##_, attr::s, attr::p>; \
    public: \
        typedef attr::Drawable<internal_shape__##n##_, attr::s, attr::p> Draw; \
        static int w () { return w_; } \
        static int h () { return h_; }

namespace Attributes {

class DrawableBase {
public:
    static double ScaleX;
    static double ScaleY;

    GC pickColor (XInfo *xinfo, Color color) {
        switch (color) {
            case BLACK:
                return xinfo->black_;
            case WHITE:
                return xinfo->white_;
            case GREY:
                return xinfo->grey_;
            case GREEN:
                return xinfo->green_;
            case BROWN:
                return xinfo->brown_;
            case RED:
                return xinfo->red_;
            default:
                error("Unknown Color");
                return xinfo->white_;       // should never reach here
        }
    }
};

template <typename D, typename S>
class DrawableImpl {};

template <typename D>
class DrawableImpl <D, Rectangle> : public DrawableBase {
public:
    typedef int (*Draw)
                (Display*, ::Drawable, GC, int, int, unsigned int, unsigned int);

    template <Color color>
    void drawImpl (XInfo *xinfo, int x, int y, Draw draw) {
        Display *display = xinfo->display_;
        Pixmap   buffer  = xinfo->buffer_;
        GC       gcColor = pickColor(xinfo, color);

        draw(display, buffer, gcColor,
             x,
             y,
             self.w() / ScaleX,
             self.h() / ScaleY);
    }
};

template <typename D>
class DrawableImpl <D, Ellipse> : public DrawableBase {
public:
    typedef int (*Draw)
                (Display*, ::Drawable, GC, int, int,
                 unsigned int, unsigned int, int, int);

    template <Color color>
    void drawImpl (XInfo *xinfo, int x, int y, Draw draw) {
        Display *display = xinfo->display_;
        Pixmap   buffer  = xinfo->buffer_;
        GC       gcColor = pickColor(xinfo, color);

        draw(display, buffer, gcColor,
             x,
             y,
             self.w() / ScaleX,
             self.h() / ScaleY,
             0, 360 * 64);
    }
};

template <typename D>
class Drawable <D, Rectangle, Border> : public DrawableImpl<D, Rectangle> {
public:
    template <Color color>
    void draw (XInfo *xinfo, int x, int y) {
        this->template drawImpl<color>(xinfo, x, y, XDrawRectangle);
    }
};

template <typename D>
class Drawable <D, Rectangle, Fill> : public DrawableImpl<D, Rectangle> {
public:
    template <Color color>
    void draw (XInfo *xinfo, int x, int y) {
        this->template drawImpl<color>(xinfo, x, y, XFillRectangle);
    }
};

template <typename D>
class Drawable <D, Ellipse, Border> : public DrawableImpl<D, Ellipse> {
public:
    template <Color color>
    void draw (XInfo *xinfo, int x, int y) {
        this->template drawImpl<color>(xinfo, x, y, XDrawArc);
    }
};

template <typename D>
class Drawable <D, Ellipse, Fill> : public DrawableImpl<D, Ellipse> {
public:
    template <Color color>
    void draw (XInfo *xinfo, int x, int y) {
        this->template drawImpl<color>(xinfo, x, y, XFillArc);
    }
};

template <typename D>
class XYCoordinates<D, IsMember> {
public:
    int x () const { return self.x_; }
    int y () const { return self.y_; }

    void setX (int x) { self.x_ = x; }
    void setY (int y) { self.y_ = y; }
};

template <typename D>
class XYCoordinates<D, IsStatic> {
public:
    static int x () { return D::x_; }
    static int y () { return D::y_; }

    static void setX (int x) { D::x_ = x; }
    static void setY (int y) { D::y_ = y; }
};

template <typename D>
class WHSizes<D, IsMember> {
public:
    int w () const { return self.w_; }
    int h () const { return self.h_; }

    void setW (int w) { self.w_ = w; }
    void setH (int h) { self.h_ = h; }
};

template <typename D>
class WHSizes<D, IsStatic> {
public:
    static int w () { return D::w_; }
    static int h () { return D::h_; }

    static void setW (int w) { D::w_ = w; }
    static void setH (int h) { D::h_ = h; }
};

template <typename D>
class Speed<D, IsMember> {
public:
    int speed () const { return self.speed_; }

    void incSpeed () { if (self.speed_ < self.speedMax_) ++self.speed_; }
    void decSpeed () { if (self.speed_ > self.speedMin_) --self.speed_; }
};

template <typename D>
class Speed<D, IsStatic> {
public:
    static int speed () { return D::speed_; }

    static void incSpeed () { if (D::speed_ < D::speedMax_) ++D::speed_; }
    static void decSpeed () { if (D::speed_ > D::speedMin_) --D::speed_; }
};

}

#undef self

#endif//ATTRIBUTES_IMPL_H
