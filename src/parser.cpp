/*-------------parser.cpp-----------------------------------------------------//
*
* Purpose: Simple commandline parsing for SciVL
*
*-----------------------------------------------------------------------------*/

#include <string>
#include <cstring>
#include <fstream>

#include <GL_setup.h>
#include <parg.h>

// This function parses arguments and was written by stek29 from simuleios 
// twitch channel chat
namespace{
    typedef struct res {
        int w;
        int h;
    } res;
    
    bool parse_res (const char *str, res &save_to) {
        const char *x_pos = strchr(str, 'x');
        
        // strchr returns NULL if not found
        if (x_pos != nullptr) {
            int width = atoi(str); 
            int height = atoi(x_pos + 1);
            
            // atoi returns 0 if string can't be parsed
            if (width > 0 && height > 0) {
                save_to.w = width;
                save_to.h = height;
                return true;
            }
        }
        
        return false;
    }
}

Param parse_args(int argc, char** argv){
    struct parg_state ps;
    int c;

    parg_init(&ps);

    Param par;

    // setting default options
    par.dist = "fourier";
    par.height = 1000;
    par.width = 1000;

    par.fullscreen = false;

    while ((c = parg_getopt(&ps, argc, argv, "s:r:hf")) != -1){
        switch(c){
            case 1: 
            {
                std::cout << "no option provided." << '\n';
                break;
            }
            case 's':
            { 
                std::string dist = ps.optarg;
                std::cout << "Simulation type is " << dist << '\n';
                par.dist = dist;
                break;
            }
            case 'h':
            {
                // read in the helpfile
                std::ifstream helpfile("src/helpfile", std::ifstream::in);
                std::string line;
                while (std::getline(helpfile, line)){
                    std::cout << line << '\n';
                }
                exit(0);
                break;
            }
            case 'f':
            {
                std::cout << "entering fullscreen mode" << '\n';
                par.fullscreen = true;
                break;
            }
            case 'r':
            {
                std::cout << "Resolution for SciVL is: " << ps.optarg << '\n';
                res parsed;
                if (parse_res(ps.optarg, parsed)) {
                    std::cout << parsed.w << 'x' << parsed.h;   
                    par.height = parsed.h;
                    par.width = parsed.w;
                } else {
                    std::cout << "invalid screen resolution, try 500x500";
                }
                std::cout << std::endl;
                break;
            }
            case '?':
            {
                if (ps.optopt == 's') {
                    printf("option -s requires an argument\n");
                }
                else {
                    printf("unknown option -%c\n", ps.optopt);
                }
                exit(1);
                break;
            }
            default:
            {
                std::cout << "ERROR: unhandles option: " << c << '\n';
                break;
            }
        }
    }

    return par;

}

