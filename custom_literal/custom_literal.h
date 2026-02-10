
namespace Chena {

struct length {
    double value;
    enum unit {
        metre,
        kilometre,
        millimetre,
        centimetre,
        inch,
        foot,
        yard,
        mile,
    };
    static constexpr double factors[] = {1.0, 1000.0, 1e-3, 1e-2, 0.0254, 0.3048, 0.9144, 1609.344};
    explicit length(double v, unit u = metre) { value = v * factors[u]; }
};

inline length operator+(length lhs, length rhs) { return length(lhs.value + rhs.value); }

}