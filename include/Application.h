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

class Application {
private:
    sf::RenderWindow window;
    sf::Color bgColor;
    SFMLEventDispatcher eventDispatcher;
    std::shared_ptr<EntityEventDispatcher> mouseEventDispatcher;
    std::shared_ptr<EntityEventDispatcher> guiEventDispatcher;
    std::shared_ptr<MousePositionProvider> mppGUI;
    std::shared_ptr<MousePositionProvider> mppWorldPos;

    sf::Font font;
    sf::View view;
    sf::View gui_view;
    Graph graph;

    void processEvents(sf::Event &event, const sf::Time &dt);

    void update(const sf::Time &dt);

    void render(const sf::Time &dt);



public:
    void run();

    Application(int width, int height, const std::string &title = "Application");



};


#endif //DISCRETEMATHLAB3_APPLICATION_H
