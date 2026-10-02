#include "visualizer.hpp"
#include <iostream>
#include <vector>
#include <cmath> 

using namespace std;

const float PI = 3.14159265f;

struct Node {
    string id;
    sf::CircleShape shape;
    sf::Text label;
};

void draw_dfa(const unordered_map<string, unordered_map<string, string>>& dfa, const unordered_set<string>& final_states, const unordered_set<string>& initial_states , string &file_name) {
    sf::RenderWindow window(sf::VideoMode(1024, 768), file_name);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("helper_modules/arial.ttf")) {
        cerr << "Error: arial.ttf not found in the executable directory." << endl;
        return;
    }

    unordered_map<string, unordered_map<string, string>> aggregated_edges;
    for (const auto& pair : dfa) {
        string from = pair.first;
        for (const auto& transition : pair.second) {
            string letter = transition.first;
            string to = transition.second;
            if (!aggregated_edges[from][to].empty()) {
                aggregated_edges[from][to] += ", " + letter;
            } else {
                aggregated_edges[from][to] = letter;
            }
        }
    }

    vector<Node> nodes;
    unordered_map<string, int> node_lookup; 
    
    int index = 0;
    float startX = 150.f, startY = 150.f;
    
    for (const auto& pair : dfa) {
        if (node_lookup.find(pair.first) == node_lookup.end()) {
            Node n;
            n.id = pair.first;
            
            n.shape.setRadius(40.f); 
            n.shape.setOutlineThickness(3.f);
            n.shape.setOrigin(40.f, 40.f); 
            n.shape.setPosition(startX, startY);

            n.label.setFont(font);
            n.label.setString(n.id);
            n.label.setCharacterSize(16);
            
            sf::FloatRect textRect = n.label.getLocalBounds();
            n.label.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

            nodes.push_back(n);
            node_lookup[n.id] = index++;
            
            startX += 200.f; 
            if (startX > 800.f) { startX = 150.f; startY += 200.f; }
        }
    }

    Node* dragged_node = nullptr;
    bool isDarkMode = false; // Tracks current theme
    bool auto_saved = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // Toggle Dark Mode
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {
                if(isDarkMode){
                    cout << "Changed from Dark Mode to Light Mode\n";
                }
                else{
                    cout << "Changed from Light Mode to Dark Mode\n";
                }
                isDarkMode = !isDarkMode;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
                sf::Texture texture;
                texture.create(window.getSize().x, window.getSize().y);
                texture.update(window);
                texture.copyToImage().saveToFile(file_name);
                cout << "Exported graph to " << file_name << endl;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                for (auto& node : nodes) {
                    if (node.shape.getGlobalBounds().contains(mousePos)) {
                        dragged_node = &node;
                        break;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                dragged_node = nullptr;
            }
        }

        if (dragged_node) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            dragged_node->shape.setPosition(mousePos);
        }

        // Define dynamic colors based on current mode
        sf::Color bgColor = isDarkMode ? sf::Color(30, 30, 30) : sf::Color::White;
        sf::Color nodeNormal = isDarkMode ? sf::Color(45, 85, 125) : sf::Color(173, 216, 230);
        sf::Color nodeSelected = isDarkMode ? sf::Color(70, 150, 70) : sf::Color(144, 238, 144);
        sf::Color nodeOutline = isDarkMode ? sf::Color(220, 220, 220) : sf::Color::Black;
        sf::Color textColor = isDarkMode ? sf::Color::White : sf::Color::Black;
        sf::Color edgeColor = isDarkMode ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100);
        sf::Color edgeTextCol = isDarkMode ? sf::Color(255, 120, 120) : sf::Color::Red;
        sf::Color initArrowCol = isDarkMode ? sf::Color(100, 200, 255) : sf::Color::Blue;

        window.clear(bgColor);

        // Draw transition edges and arrows
        for (const auto& from_pair : aggregated_edges) {
            string from_id = from_pair.first;
            if (node_lookup.find(from_id) == node_lookup.end()) continue;
            
            for (const auto& to_pair : from_pair.second) {
                string to_id = to_pair.first;
                string labels = to_pair.second;
                if (node_lookup.find(to_id) == node_lookup.end()) continue;

                sf::Vector2f p1 = nodes[node_lookup[from_id]].shape.getPosition();
                sf::Vector2f p2 = nodes[node_lookup[to_id]].shape.getPosition();

                sf::Text edgeText(labels, font, 18);
                edgeText.setFillColor(edgeTextCol);

                if (from_id == to_id) {
                    sf::CircleShape self_loop(25.f);
                    self_loop.setFillColor(sf::Color::Transparent);
                    self_loop.setOutlineThickness(2.f);
                    self_loop.setOutlineColor(edgeColor);
                    self_loop.setOrigin(25.f, 25.f);
                    self_loop.setPosition(p1.x, p1.y - 50.f); 
                    window.draw(self_loop);

                    sf::ConvexShape loop_arrow;
                    loop_arrow.setPointCount(3);
                    loop_arrow.setPoint(0, sf::Vector2f(0.f, -6.f));
                    loop_arrow.setPoint(1, sf::Vector2f(0.f, 6.f));
                    loop_arrow.setPoint(2, sf::Vector2f(12.f, 0.f));
                    loop_arrow.setFillColor(edgeColor);
                    loop_arrow.setOrigin(12.f, 0.f);
                    loop_arrow.setPosition(p1.x + 25.f, p1.y - 50.f); 
                    loop_arrow.setRotation(90.f); 
                    window.draw(loop_arrow);

                    edgeText.setPosition(p1.x - 10.f, p1.y - 95.f);
                    window.draw(edgeText);
                    continue;
                }

                sf::Vertex line[] = {
                    sf::Vertex(p1, edgeColor),
                    sf::Vertex(p2, edgeColor)
                };
                window.draw(line, 2, sf::Lines);

                float dx = p2.x - p1.x;
                float dy = p2.y - p1.y;
                float distance = std::sqrt(dx * dx + dy * dy);
                
                if (distance > 0) {
                    float angle = std::atan2(dy, dx) * 180.f / PI;
                    sf::Vector2f arrowPos(p2.x - (dx / distance) * 40.f, p2.y - (dy / distance) * 40.f);

                    sf::ConvexShape arrow;
                    arrow.setPointCount(3);
                    arrow.setPoint(0, sf::Vector2f(0.f, -8.f));
                    arrow.setPoint(1, sf::Vector2f(0.f, 8.f));
                    arrow.setPoint(2, sf::Vector2f(16.f, 0.f));
                    arrow.setFillColor(edgeColor);
                    arrow.setOrigin(16.f, 0.f);
                    arrow.setPosition(arrowPos);
                    arrow.setRotation(angle);
                    
                    window.draw(arrow);
                }

                sf::Vector2f midPoint((p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f);
                edgeText.setPosition(midPoint.x, midPoint.y - 20.f); 
                window.draw(edgeText);
            }
        }

        // Draw nodes
        for (auto& node : nodes) {
            sf::Vector2f pos = node.shape.getPosition();
            
            // Apply dynamic colors
            node.shape.setFillColor((&node == dragged_node) ? nodeSelected : nodeNormal);
            node.shape.setOutlineColor(nodeOutline);
            node.label.setFillColor(textColor);

            if (initial_states.find(node.id) != initial_states.end()) {
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(pos.x - 90.f, pos.y), initArrowCol),
                    sf::Vertex(sf::Vector2f(pos.x - 40.f, pos.y), initArrowCol)
                };
                window.draw(line, 2, sf::Lines);

                sf::ConvexShape arrow_head;
                arrow_head.setPointCount(3);
                arrow_head.setPoint(0, sf::Vector2f(0.f, -10.f));
                arrow_head.setPoint(1, sf::Vector2f(0.f, 10.f));
                arrow_head.setPoint(2, sf::Vector2f(15.f, 0.f));
                arrow_head.setFillColor(initArrowCol);
                arrow_head.setOrigin(15.f, 0.f);
                arrow_head.setPosition(pos.x - 40.f, pos.y);
                window.draw(arrow_head);
            }

            window.draw(node.shape);

            if (final_states.find(node.id) != final_states.end()) {
                sf::CircleShape inner_circle(32.f); 
                inner_circle.setOrigin(32.f, 32.f);
                inner_circle.setPosition(pos);
                inner_circle.setFillColor(sf::Color::Transparent);
                inner_circle.setOutlineThickness(2.f);
                inner_circle.setOutlineColor(nodeOutline);
                window.draw(inner_circle);
            }

            node.label.setPosition(pos);
            window.draw(node.label);
        }

        window.display();
        
        if (!auto_saved) {
            sf::Texture texture;
            texture.create(window.getSize().x, window.getSize().y);
            texture.update(window);
            texture.copyToImage().saveToFile(file_name);
            cout << "Exported graph to " << file_name << endl;
            auto_saved = true;
        }
    }
}