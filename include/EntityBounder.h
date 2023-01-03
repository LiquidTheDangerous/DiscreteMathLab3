//
// Created by ASUS on 03.01.2023.
//

#ifndef DISCRETEMATHLAB3_ENTITYBOUNDER_H
#define DISCRETEMATHLAB3_ENTITYBOUNDER_H
#include <Arrow.h>

class Application;
class EntityBounder {
public:
    explicit EntityBounder(Application* application);
    void pushEntity(Entity* e);
private:

    Entity* first;
    Entity* second;
    Application* app;
};


#endif //DISCRETEMATHLAB3_ENTITYBOUNDER_H
