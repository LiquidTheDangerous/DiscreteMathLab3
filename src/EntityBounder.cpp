//
// Created by ASUS on 03.01.2023.
//

#include <EntityBounder.h>
#include <Application.h>
EntityBounder::EntityBounder(Application* app){
        this->first = nullptr;
        this->second = nullptr;
        this->app = app;
}

void EntityBounder::pushEntity(Entity *e) {
        if (this->first){
            this->second = e;
            auto arrow = std::make_shared<Arrow>(first,second);
            app->mouseEventDispatcher->addEntity(arrow);
            app->createMessage("Edge created",0.5f);

            this->first = nullptr;
            this->second = nullptr;
        }
        else{
            this->first = e;
        }
}
