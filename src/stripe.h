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
    int wallType;

    Stripe(int left, int strip_width);
    ~Stripe() {};
};

#endif //RAYCASTING_STRIPE_H
