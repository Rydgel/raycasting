#import "stripe.h"


Stripe::Stripe(int left, int strip_width)
{
    this->top = 0;
    this->left = left;
    this->height = 0;
    this->width = strip_width;
    this->xHit = 0;
    this->yHit = 0;
    this->wallIsHorizontal = false;
    this->wallType = 0;
}