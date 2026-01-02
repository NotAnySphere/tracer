#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <functional>
#include <iostream>

#include "hittable.h"
#include "tri.h"

/*
only supports

v ...
f 1 2 3 

*/

enum class parse_error
{
    unrecognized_line,
    value_parse_error
};

class object {
    public:
        std::vector<std::unique_ptr<hittable>> obj;

        object(std::vector<std::unique_ptr<hittable>> hittables) : obj(hittables) {}

        box aabb() {            
            if (obj.size() < 1)
            {
                std::cout << "empty object!" << std::endl;
            }
            
            box aabb = obj[0]->aabb();
            for (auto &&i : obj)
            {
                aabb = box(aabb.aabb(), i->aabb());
            }
            return aabb;
        }

        // void scale_by(double factor) {}; ... and others like this
};

struct face {
    uint16_t v1;
    uint16_t v2;
    uint16_t v3;
};

class obj {
    public:
        std::vector<point3> vert = {};
        std::vector<face> face = {};

        point3 get_vert(const std::vector<std::string> verts) {
            // index 0 is "v"
            return point3(std::stod(verts[1]), std::stod(verts[2]), std::stod(verts[3]));
        }

        struct face get_face(const std::vector<std::string> face_index) {
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

            for (auto &&i : this->face)
            {
                point3 v1 = vert[i.v1];
                point3 v2 = vert[i.v2];
                point3 v3 = vert[i.v3];
                std::array<point3, 3> points = { v1, v2, v3 };
                tris.push_back( make_unique<tri>(points) );
            }
            

            return tris;
        }
};

class obj_parse_result {
    std::vector<point3> result;
};

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

auto get_obj(std::istream file) -> void
{
    obj parsed = {};
    
    std::string read_line;
    while (getline(file, read_line))
    {
        parsed.line(read_line);
    }
    
    if (file.bad())
    {
        std::cout << "error reading file" << std::endl;
    }
}

auto load(std::string filename) -> void
{
    auto dot_pos = filename.find_last_of('.');
    filename.substr(dot_pos + 1, filename.length() - 1 - dot_pos);
}
