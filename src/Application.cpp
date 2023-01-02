//
// Created by ASUS on 30.12.2022.
//

#include <iostream>
#include <Application.h>
#include <MousePositionProviderImpl.h>
#include <Label.h>
#include <Message.h>

Application::Application(int width, int height, const std::string &title) :
        window(sf::VideoMode(width, height), title),
        bgColor(sf::Color::White) {
    this->mouseEventDispatcher = std::make_shared<EntityEventDispatcherImpl>();
    this->guiEventDispatcher = std::make_shared<EntityEventDispatcherImpl>();
    this->window.setVerticalSyncEnabled(true);
    this->view = this->window.getDefaultView();
    this->gui_view = this->window.getDefaultView();
    this->mppGUI = std::make_shared<MousePositionProviderImpl>(&this->window, &this->gui_view);
    this->mppWorldPos = std::make_shared<MousePositionProviderImpl>(&this->window, &this->view);

    if (!this->font.loadFromFile("../res/Montserrat_Regular.ttf")) {
        std::cerr << "ERROR LOADING FONT" << std::endl;
    };
    this->eventDispatcher.addListenerOnEvent(sf::Event::Resized,
                                             [this](const sf::Event &event, const sf::Time &dt) {


                                                 this->gui_view.setSize(event.size.width, event.size.height);
                                                 this->gui_view.setCenter(event.size.width / 2, event.size.height / 2);
                                                 this->view.setSize(event.size.width, event.size.height);
                                                 this->view.setCenter(event.size.width / 2, event.size.height / 2);

                                             });



    auto e = std::make_shared<VertexEntity>(this->font, "123", mppWorldPos);
    e->move(100, 100);

    auto label = std::make_shared<Label>(this->font, "Enter Vertex Name");
    label->move(600, 0);
    label->getSignal(signals::onEndEditingText).addSlot([label,this](void* param)
    {
        auto text = label->getString();
        label->setString("");
        auto e = std::make_shared<VertexEntity>(this->font, text, this->mppWorldPos);
        e->followMouse(0,0);
        this->mouseEventDispatcher->addEntity(e);
    });

    auto m = std::make_shared<Message>(this->font,"test",3);
    m->move(100,100);
    m->getSignal(signals::onDelete).addSlot([](void*)
    {
        std::cout << "RM" << std::endl;
    });
    this->mouseEventDispatcher->setMousePositionProvider(mppWorldPos);

    this->guiEventDispatcher->setMousePositionProvider(mppGUI);

    this->mouseEventDispatcher->addEntity(e);
    this->guiEventDispatcher->addEntity(label);
    this->guiEventDispatcher->addEntity(m);
}

void Application::run() {
    sf::Event event{};
    sf::Clock clock{};
    sf::Time dt{};
    while (this->window.isOpen()) {
        dt = clock.restart();
        this->processEvents(event, dt);
        this->update(dt);
        this->render(dt);
    }
}


void Application::processEvents(sf::Event &event, const sf::Time &dt) {
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->window.close();
        }
        this->eventDispatcher.handleEvent(event, dt);
        this->mouseEventDispatcher->handleEvent(event);
        this->guiEventDispatcher->handleEvent(event);
    };
    this->eventDispatcher.handleInput(dt);
}

void Application::update(const sf::Time &dt) {

    this->mouseEventDispatcher->update(dt.asSeconds());
    this->guiEventDispatcher->update(dt.asSeconds());
}

void Application::render(const sf::Time &dt) {
    this->window.clear(this->bgColor);
    this->window.setView(this->view);
    this->window.draw(*this->mouseEventDispatcher);


    this->window.setView(this->gui_view);
    this->window.draw(*this->guiEventDispatcher);

    this->window.display();
}
