//
// Created by ASUS on 03.01.2023.
//

#ifndef DISCRETEMATHLAB3_COLORHELPERS_H
#define DISCRETEMATHLAB3_COLORHELPERS_H

#include <cstdint>
#include <SFML/Graphics.hpp>

namespace ColorHelpers {

    struct ColorRGB {
        std::uint8_t r{0};
        std::uint8_t g{0};
        std::uint8_t b{0};
        ColorRGB() = default;
        ColorRGB(const sf::Color& color){
            this->r = color.r;
            this->g = color.g;
            this->b = color.b;
        }
        operator sf::Color() const{
            return {r,g,b};
        }
    };

    struct ColorHSV {
        std::uint8_t h{0};
        std::uint8_t s{0};
        std::uint8_t v{0};
    };


    ColorHSV convertRGB2HSV(const ColorRGB &in) {
        ColorHSV out;
        double min, max, delta;

        min = in.r < in.g ? in.r : in.g;
        min = min < in.b ? min : in.b;

        max = in.r > in.g ? in.r : in.g;
        max = max > in.b ? max : in.b;

        out.v = max;                                // v
        delta = max - min;
        if (delta < 0.00001) {
            out.s = 0;
            out.h = 0; // undefined, maybe nan?
            return out;
        }
        if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
            out.s = (delta / max);                  // s
        } else {
            // if max is 0, then r = g = b = 0
            // s = 0, h is undefined
            out.s = 0.0;
            out.h = -1;                            // its now undefined
            return out;
        }
        if (in.r >= max)                           // > is bogus, just keeps compilor happy
            out.h = (in.g - in.b) / delta;        // between yellow & magenta
        else if (in.g >= max)
            out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
        else
            out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

        out.h *= 60.0;                              // degrees

        if (out.h < 0.0)
            out.h += 360.0;

        return out;
    };

    ColorRGB convertHSV2RGB(const ColorHSV& in) {
        double hh, p, q, t, ff;
        long i;
        ColorRGB out;

        if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
            out.r = in.v;
            out.g = in.v;
            out.b = in.v;
            return out;
        }
        hh = in.h;
        if (hh >= 360.0) hh = 0.0;
        hh /= 60.0;
        i = (long) hh;
        ff = hh - i;
        p = in.v * (1.0 - in.s);
        q = in.v * (1.0 - (in.s * ff));
        t = in.v * (1.0 - (in.s * (1.0 - ff)));

        switch (i) {
            case 0:
                out.r = in.v;
                out.g = t;
                out.b = p;
                break;
            case 1:
                out.r = q;
                out.g = in.v;
                out.b = p;
                break;
            case 2:
                out.r = p;
                out.g = in.v;
                out.b = t;
                break;

            case 3:
                out.r = p;
                out.g = q;
                out.b = in.v;
                break;
            case 4:
                out.r = t;
                out.g = p;
                out.b = in.v;
                break;
            case 5:
            default:
                out.r = in.v;
                out.g = p;
                out.b = q;
                break;
        }
        return out;
    };

    template<typename F>
    ColorRGB interpolate(ColorRGB a, ColorRGB b, float t, F interpolator) {
        // 0.0 <= t <= 1.0
        ColorHSV ca = convertRGB2HSV(a);
        ColorHSV cb = convertRGB2HSV(b);
        ColorHSV final;

        final.h = interpolator(ca.h, cb.h, t);
        final.s = interpolator(ca.s, cb.s, t);
        final.v = interpolator(ca.v, cb.v, t);

        return convertHSV2RGB(final);
    }

    int linear(int a, int b, float t) {
        return a * (1 - t) + b * t;
    }

// use: result = interpolate(color1,color2,ratio,&linear);
};


#endif //DISCRETEMATHLAB3_COLORHELPERS_H
