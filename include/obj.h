#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <functional>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <charconv>

#include "../include/hittable.h"
#include "../include/hittable_list.h"
#include "../include/tri.h"
#include "../include/box.h"
#include "../include/aabb_bvh.h"
#include "../include/utils/arena.hpp"
#include "../include/utils/logger_alloc.hpp"

/*
only supports

v ...
f 1 2 3 

*/

// using String = std::basic_string<CharT, Traits, Alloc>;

template<class CharT, class Traits, class Alloc>
auto carriage(std::basic_string<CharT, Traits, Alloc>& str) -> std::basic_string<CharT, Traits, Alloc>
{
    if (str[str.size() - 1] == '\n') {
        return str.substr(0, str.length() - 1);
    }
    return str;
}
template<class CharT, class Traits, class Alloc>
auto leading_spaces(std::basic_string<CharT, Traits, Alloc>& str) -> std::basic_string<CharT, Traits, Alloc>
{
    auto first = str.find_first_not_of(' ');
    auto chopped = str.substr(first, str.length() - first);
    return chopped;
}

template<typename Pred, typename Container>
auto filter(Pred pred, Container& list)
{
    Container filtered = {};

    for (auto &&i : list)
    {
        if (pred(i))
        {
            filtered.push_back(i);
        }
    }
    return filtered;
}

template<class CharT, class Traits, class Alloc>
auto split(const std::string& split, const std::basic_string<CharT, Traits, Alloc>& line) -> std::vector<std::basic_string<CharT, Traits, Alloc>>
{
    std::vector<std::basic_string<CharT, Traits, Alloc>> subs = {};
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
        std::vector<hittable*> obj{};

        object(std::vector<hittable*> hittables) : obj(hittables) {}

        object() {
            throw "Empty object being created...";
            // obj.push_back(&box());
        }
        /*
        */

        std::vector<hittable*> list() {
            return obj;
        }

        aabb_bvh* bvh(arena* alloc) {
            auto list = this->list();
            return alloc->emplace_item<aabb_bvh>(alloc, list, 0, list.size());
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

        template <typename View>
        point3 get_vert(const View& verts) {
            double x,y,z;
            // index 0 is "v"
            std::from_chars(verts[1].data(), verts[1].data() + verts[1].size(), x);
            std::from_chars(verts[2].data(), verts[2].data() + verts[2].size(), y);
            std::from_chars(verts[3].data(), verts[3].data() + verts[3].size(), z);
            return point3(x, y, z);
        }

        template <typename View>
        struct face get_face(const View& face_index) {
            uint16_t v1,v2,v3;
            // index 0 is "f"
            std::from_chars(face_index[1].data(), face_index[1].data() + face_index[1].size(), v1);
            std::from_chars(face_index[2].data(), face_index[2].data() + face_index[2].size(), v2);
            std::from_chars(face_index[3].data(), face_index[3].data() + face_index[3].size(), v3);
            return face {v1, v2, v3};
        }

        template<class CharT, class Traits, class Alloc>
        void line(std::basic_string<CharT, Traits, Alloc>& line) {
            //std::cout << line << "\n";
            auto crlf = carriage(line);
            auto leading = leading_spaces(crlf);
            auto splitted = split(" ", leading);
            auto words = filter([] (auto& str) { return !(str.empty() || str[0] == ' '); }, splitted);
                        
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
                break;            
            default:
                break;
            }

        }

        object to_object(arena* alloc) {
            std::vector<hittable*> tris = {};

            for (auto &&i : this->faces)
            {
                point3 v1 = verts[i.v1 - 1]; // .obj vert is 1-based index
                point3 v2 = verts[i.v2 - 1];
                point3 v3 = verts[i.v3 - 1];
                std::array<point3, 3> points = { v1, v2, v3 };                
                auto ptr = alloc->emplace_item<tri>(points);

                tris.push_back( ptr );
            }

            return tris;
        }
};


auto get_obj(std::ifstream& file) -> obj
{
    obj parsed = obj();
    
    //std::string read_line;
    //std::basic_string<char, std::char_traits<char>, LoggingAllocator<char>> read_line {};

    ArenaAllocator<char> alloc = ArenaAllocator<char>();
    std::basic_string<char, std::char_traits<char>, ArenaAllocator<char>> read_line("tst",alloc);
    
    while (true)
    {
        alloc.clear();
        if (!getline(file, read_line)) {
            break;
        }
        parsed.line(read_line); 
    }
    return parsed;
}

auto load(std::string filename, arena* alloc) -> object
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
        return obj.to_object(alloc);
    } else {
        std::cout << "unrecognized file postfix, filename: " << filename << std::endl;
        return object();
    }
    return object();
    
}
