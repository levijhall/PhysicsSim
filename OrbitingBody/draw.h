#pragma once

const double pi_d = 3.14159265358979323846;
const float pi = (float) pi_d;

void drawCircle(vec<2> center, double radius, unsigned sides = 32);
void drawCircleFilled(vec<2> center, double radius, unsigned sides = 32);
void drawDisc(vec<2> center, double r, double R, vec<4> color1, vec<4> color2, unsigned sides = 32, double rotation = 0.f);
void drawArrow(vec<2> begin, vec<2> end, double size = 1.f);