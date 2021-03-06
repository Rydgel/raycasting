#ifndef RAYCASTING_STRIPE_H
#define RAYCASTING_STRIPE_H

class Stripe {
public:

    double top;
    double left;
    double width;
    double height;
    double xHit;
    double yHit;
    double dist;
    bool wallIsHorizontal;
    int wallType;

    Stripe(const int left, const int strip_width);
    ~Stripe() {};
};

#endif //RAYCASTING_STRIPE_H
