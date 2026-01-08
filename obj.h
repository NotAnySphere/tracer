#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <functional>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "hittable.h"
#include "hittable_list.h"
#include "tri.h"
#include "box.h"
#include "aabb_bvh.h"

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
            subs.push_back(line.substr(pos, (line.length() - pos)));
            break;
        }
        subs.push_back(line.substr(pos, split_pos - pos));
        pos = split_pos + 1;
    }
    return subs;
}


class object {
    public:
        std::vector<std::unique_ptr<hittable>> obj;

        object(std::vector<std::unique_ptr<hittable>> hittables) : obj(std::move(hittables)) {}

        object() {
            std::vector<std::unique_ptr<hittable>> list = {};
            list.push_back(make_unique<box>());
        }

        std::vector<std::unique_ptr<hittable>> list() {
            return std::move(obj);
        }

        aabb_bvh bvh() {
            auto list = this->list();
            return aabb_bvh(list, 0, list.size());
        }

        box aabb() {
            if (obj.size() < 1)
            {
                return box();
            }
            
            box aabb = obj[0]->aabb();
            for (size_t i = 0; i < obj.size(); i++)
            {
                aabb = box(aabb, obj[i]->aabb());
            }
            return aabb;
        }

        void scale_by(double factor) {
            // get lowest point of bounding box, scale and then translate back to same reference point
            box bb1 = aabb();
            point3 p11 = bb1.p1;
            for (auto &&i : obj)
            {
                i->scale_by(factor);
            }
            box bb2 = aabb();
            point3 p12 = bb2.p1;
            translate_by(p11 - p12);
        }

        void translate_by(vec3 vec) {
            for (auto &&i : obj)
            {
                i->translate_by(vec);
            }
        }
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
            std::cout << "get faces: " << face_index[1] << face_index[2] << face_index[3] << std::endl;
            
            struct face face = {(uint16_t) std::stoi(face_index[1]),
                                (uint16_t) std::stoi(face_index[2]),
                                (uint16_t) std::stoi(face_index[3])};
            std::cout << "got faces: " << face.v1 << face.v2 << face.v3 << std::endl;
                            
            return face;
        }

        void line(std::string line) {
            std::cout << "parsing line: " << line << std::endl;
            
            auto leading = leading_spaces(line);
            auto splitted = split(" ", leading);
            auto words = filter([] (std::string str) { return !(str.empty() || str[0] == ' '); }, splitted);
            
            std::cout << "got parsed: " << words.size() << std::endl;
            
            if (words.size() < 2)
            {
                return;
            }
            switch (words[0][0])
            {
            case '#':
                break;
            case 'v':
                this->verts.push_back(get_vert(words));
                break;
            case 'f':
                this->faces.push_back(get_face(words));
                std::cout << faces[faces.size()].v1 <<faces[faces.size()].v2 <<faces[faces.size()].v3 << std::endl;
                break;            
            default:
                break;
            }

        }

        object to_object() {
            std::vector<std::unique_ptr<hittable>> tris = {};

            for (auto &&i : this->faces)
            {
                point3 v1 = verts[i.v1 - 1]; // .obj vert is 1-based index
                point3 v2 = verts[i.v2 - 1];
                point3 v3 = verts[i.v3 - 1];
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
    std::cout << "current working dir: " << std::filesystem::current_path() << std::endl;
    if (!std::filesystem::exists(filename))
    {
        std::cout << "file doesn't exist, filename: " << filename << std::endl;
        return object();
    }
    
    auto dot_pos = filename.find_last_of('.');
    std::string postfix = filename.substr(dot_pos + 1, filename.length() - 1 - dot_pos);
    
    std::ifstream file;
    file.open(filename, std::ifstream::in);
    
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

    if (postfix.compare("obj") == 0)
    {
        auto obj = get_obj(file);
        return obj.to_object();
    } else {
        std::cout << "unrecognized file postfix, filename: " << filename << std::endl;
        return object();
    }
    return object();
    
}
