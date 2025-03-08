#pragma once

class UI {
public:
    static void clearScreen();
    static void title();
    static void drawHistogram(double percentages[], int size);
    static void displayRatingVisual(double avgRating);
};
