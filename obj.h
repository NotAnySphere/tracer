#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <functional>
#include <iostream>
#include <fstream>

#include "hittable.h"
#include "hittable_list.h"
#include "tri.h"
#include "box.h"

/*
only supports

v ...
f 1 2 3 

*/

auto leading_spaces(const std::string str) -> std::string
{
    auto first = str.find_first_not_of(' ');
    return str.substr(first, str.length() - 1 - first);
}

auto filter(std::function<bool(std::string)> pred, const std::vector<std::string> list) -> std::vector<std::string>
{
    std::vector<std::string> filtered = {};

    for (auto &&i : list)
    {
        if (pred(i))
        {
            filtered.push_back(i);
        }
    }
    return filtered;
}

auto split(const std::string split, const std::string line) -> std::vector<std::string>
{
    std::vector<std::string> subs = {};
    std::size_t pos = 0;

    while (true)
    {
        std::size_t split_pos = line.find(split, pos);
        if (split_pos == std::string::npos)
        {
            subs.push_back(line.substr(pos + 1, (line.length() - 1) - pos));
            break;
        }
        subs.push_back(line.substr(pos + 1, split_pos - pos - 1));
        pos = split_pos;
    }
    return subs;
}


class object {
    public:
        std::vector<std::unique_ptr<hittable>> obj;

        object(std::vector<std::unique_ptr<hittable>> hittables) : obj(hittables) {}

        object() {
            std::vector<std::unique_ptr<hittable>> list = {};
            list.push_back(make_unique<box>());
        }

        hittable_list list() {
            auto list = hittable_list();

            for (auto &&i : obj)
            {
                list.add(std::move(i));
            }

            return list;
        }

        /*
        box aabb() const override {            
            if (obj.size() < 1)
            {
                std::cout << "empty object!" << std::endl;
                return box();
            }
            
            box aabb = obj[0]->aabb();
            for (auto &&i : obj)
            {
                aabb = box(aabb, i->aabb());
            }
            return aabb;
        }
        */

        // void scale_by(double factor) {}; ... and others like this
};

struct face {
    uint16_t v1;
    uint16_t v2;
    uint16_t v3;
};

class obj {
    public:
        std::vector<point3> verts = {};
        std::vector<face> faces = {};

        point3 get_vert(const std::vector<std::string> verts) {
            // index 0 is "v"
            return point3(std::stod(verts[1]), std::stod(verts[2]), std::stod(verts[3]));
        }

        struct face get_face(const std::vector<std::string> face_index) {
            // index 0 is "f"
            struct face face = { std::stoi(face_index[1]), std::stoi(face_index[2]), std::stoi(face_index[3]) };
            return face;
        }

        void line(std::string line) {
            auto words = filter([] (std::string str) { return str.empty() || str[0] == ' '; },
                         split(" ",
                         leading_spaces(line)));
            if (words.size() < 2)
            {
                return;
            }

            switch (words[0][0])
            {
            case '#':
                break;
            case 'v':
                this->get_vert(words);
                break;
            case 'f':
                this->get_face(words);
                break;            
            default:
                break;
            }
        }

        object to_object() {
            std::vector<std::unique_ptr<hittable>> tris = {};

            for (auto &&i : this->faces)
            {
                point3 v1 = verts[i.v1];
                point3 v2 = verts[i.v2];
                point3 v3 = verts[i.v3];
                std::array<point3, 3> points = { v1, v2, v3 };
                tris.push_back( make_unique<tri>(points) );
            }

            return tris;
        }
};


auto get_obj(std::ifstream& file) -> obj
{
    obj parsed = {};
    
    std::string read_line;
    while (getline(file, read_line))
    {
        parsed.line(read_line);
    }
    return parsed;
}

auto load(std::string filename) -> object
{
    auto dot_pos = filename.find_last_of('.');
    std::string postfix = filename.substr(dot_pos + 1, filename.length() - 1 - dot_pos);
    
    std::ifstream file;
    file.open("bunny.obj", std::ifstream::in);
    
    if (!file.is_open())
    {
        file.close();
        std::cout << "couldn't open file, filename: " << filename << std::endl;
        return object();
    }

    if (file.bad())
    {
        std::cout << "error reading, bad file, filename: " << filename << std::endl;
        return object();
    }

    if (postfix.compare("obj"))
    {
        auto obj = get_obj(file);
        return obj.to_object();
    }
    return object();
    
}
