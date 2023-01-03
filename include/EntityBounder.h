//
// Created by ASUS on 03.01.2023.
//

#ifndef DISCRETEMATHLAB3_ENTITYBOUNDER_H
#define DISCRETEMATHLAB3_ENTITYBOUNDER_H
#include <Arrow.h>
class Application;
class EntityBounder {
public:
    friend class Application;
    explicit EntityBounder(Application* application);
    void pushEntity(Entity* e);
    bool isBounding() const;
private:

    Entity* first;
    Entity* second;
    Application* app;
};


#endif //DISCRETEMATHLAB3_ENTITYBOUNDER_H
