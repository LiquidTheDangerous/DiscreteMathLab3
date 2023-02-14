//
// Created by ASUS on 30.12.2022.
//

#ifndef DISCRETEMATHLAB3_APPLICATION_H
#define DISCRETEMATHLAB3_APPLICATION_H

#include <SFML/Graphics.hpp>
#include <SFMLEventDispatcher.h>
#include <VertexEntity.h>
#include <EntityEventDispatcherImpl.h>
#include <Graph.h>
#include <Message.h>
#include <EntityBounder.h>
#include <Button.hpp>
#include "Label.h"

class Application {
private:
    sf::RenderWindow window;
    sf::Color bgColor;
    SFMLEventDispatcher eventDispatcher;
    std::shared_ptr<EntityEventDispatcherImpl> mouseEventDispatcher;
    std::shared_ptr<EntityEventDispatcherImpl> guiEventDispatcher;
    std::shared_ptr<EntityEventDispatcherImpl> arrowHolder;
    std::shared_ptr<MousePositionProvider> mppGUI;
    std::shared_ptr<MousePositionProvider> mppWorldPos;

    sf::Font font;
    sf::View view;
    sf::View gui_view;
    Graph graph;

    void processEvents(sf::Event &event, const sf::Time &dt);

    void update(const sf::Time &dt);

    void render(const sf::Time &dt);

    void createMessage(const std::string &text, float ttl);

    void onSortBtnClicked(void *);

    struct ByPressingRightMouseButtonOnVertex {

        ByPressingRightMouseButtonOnVertex(Application *app, Entity *e) {
            this->app = app;
            this->e = e;
        }

        void operator()(void *param) {
            if (app->entityBounder->isBounding()) {
                app->createMessage("Cannot remove vertex while creating edge", 0.5f);
                return;
            }
            e->markToRemove(true);
            app->graph.removeVertex((e->getName()));
            app->createMessage("Vertex removed", 0.5f);
            app->colorizeVertices();
        };
        Application *app;
        Entity *e;
    };

    friend class EntityBounder;

    void updateArrows(const sf::Time &dt);

    void drawArrows();

    std::shared_ptr<EntityBounder> entityBounder;
    std::list<std::shared_ptr<Arrow>> arrows;

    void colorizeVertices() const;

    float viewMoveSpeed;
    float viewMouseScrollSpeed;
    float viewRotationSpeed;
    std::shared_ptr<Label> label;
    sf::Color pathColor;
    sf::Color defaultColor;

public:
    void run();

    Application(int width, int height, const std::string &title = "Application");

    virtual ~Application();

    void imguiWindow();

    void onLabelTextEntered(void *param);

    void createVertexByName(const std::string &text);

    void setPathColor(std::list<std::string> &path);

    void setPathColor(const std::list<std::pair<std::string, std::string>> &list);
};


#endif //DISCRETEMATHLAB3_APPLICATION_H
