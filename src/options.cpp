#include "options.h"

Options* Options::Instance = 0;

Options* Options::getInstance()
{
    if(!Instance)
    {
        Instance = new Options;
        return Instance;
    }
    else
    {
        return Instance;
    }
}

Options::Options()
{
    opt_verbose = false;
    opt_get = false;
    opt_x = 0;
    opt_y = 0;
    opt_z = 0;
    opt_c = 0;
    opt_curve = "";
    opt_defaults = false;
    opt_calibrate = false;
    opt_setRGB = false;
    opt_list = false;
}

void Options::printHelp()
{
    cout << "Option summary" << endl;


    cout << "\t-h, --help\t\t Display help" << endl;
    cout << "\t-v, --verbose\t\t Display verbose output" << endl;
    cout << "\t-l\t\t List supported devices" << endl;
    cout << "\t-d [device]\t\t Select device" << endl;
    cout << "\t--rgb [R,G,B]\t\t Set RGB lighting color [0-255]" << endl;
    cout << "\t--get\t\t\t Display current configuration" << endl;
    cout << "\t-a [axis]\t\t Select axis or axes (comma separated)" << endl;
    cout << "\t-x [value]\t\t Set the x saturation value [0-1000]" << endl;
    cout << "\t-y [value]\t\t Set the y saturation value [0-1000]" << endl;
    cout << "\t-c [value]\t\t Set the curvature value [0-1000]" << endl;
    cout << "\t-z, --deadzone [value]\t Set the deadzone value [0-1000]" << endl;
    cout << "\t--curve-type [value]\t Set the curve type [j or s]" << endl;
    cout << "\t--defaults\t\t Reset configuration to default" << endl;
    cout << "\t--calibrate\t\t Recalibrate the hall effect sensors and reset config" << endl;
    exit(0);
}

void Options::processArguments(int argc, char* argv[])
{
    static struct option long_options[] =
    {
        {"help", no_argument, 0, 'h'},
        {"verbose", no_argument, 0, 'v'},
        {"deadzone", required_argument, 0, 'z'},
        {"rgb", required_argument, 0, 3},
        {"get", no_argument, 0, 4},
        {"curve-type", required_argument, 0, 5},
        {"defaults", no_argument, 0, 6},
        {"calibrate", no_argument, 0, 7},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;
    int value = 0;
    bool setting_specified = false;
    bool device_selected = false;
    bool axis_selected = false;
    bool require_axis = false;

    while(c != -1)
    {
        c = getopt_long (argc, argv, "hlva:x:y:d:c:z:", long_options, &option_index);
        switch(c)
        {
            case 'h': // -h, --help
                this->printHelp();
                break;

            case 'v': // -v, --verbose
                this->opt_verbose = true;
                break;

            case 'l': // -l
                this->opt_list = true;
                break;

            case 'z': // -z, --deadzone
            {
                setting_specified = true;
                require_axis = true;
                value = stoi(string(optarg));
                if (value >= 0 && value <= 1000)
                {
                    this->opt_z = value;
                    break;
                }
                else
                {
                    cerr << "Specified deadzone is out of range (0-1000)." << endl;
                    exit(-1);
                }
                break;
            }

            case 3: // --rgb
            {
                setting_specified = true;
                string a = string(optarg);
                stringstream ss(a);
                string token;
                vector<uint8_t> rgb;
                while(getline(ss, token, ','))
                {
                    int num = stoi(token);
                    if (num < 0 || num > 255)
                    {
                        cerr << "RGB range is invalid, use 0-255 for each primary color, I.E (0,100,255)" << endl;
                        exit(-1);
                    }
                    rgb.push_back(num);
                }
                if (rgb.size() != 3)
                {
                    cerr << "Malformed input, need three comma separated RGB values, I.E (0,100,255)" << endl;
                    exit(-1);
                }
                this->opt_rgb = rgb;
                this->opt_setRGB = true;
                break;
            }

            case 4: // --get
                setting_specified = true;
                this->opt_get = true;
                break;

            case 5: // --curve-type
            {
                setting_specified = true;
                require_axis = true;
                string s = string(optarg);
                transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
                if (s.compare(string("s")) != 0 && s.compare(string("j")) != 0)
                {
                    cerr << "Must specify 's' or 'j' for curve type" << endl;
                    exit(-1);
                }
                this->opt_curve = s;
                break;
            }

            case 6: // --defaults
                setting_specified = true;
                this->opt_defaults = true;
                break;

            case 7: // --calibrate
                setting_specified = true;
                this->opt_calibrate = true;
                break;

            case 'x': // -x
            {
                setting_specified = true;
                require_axis = true;
                value = stoi(string(optarg));
                if (value >= 0 && value <= 1000)
                {
                    this->opt_x = value;
                    break;
                }
                else
                {
                    cerr << "Specified x saturation is out of range (0-1000)." << endl;
                    exit(-1);
                }
                break;
            }

            case 'y': // -y
            {
                setting_specified = true;
                require_axis = true;
                value = stoi(string(optarg));
                if (value >= 0 && value <= 1000)
                {
                    this->opt_y = value;
                    break;
                }
                else
                {
                    cerr << "Specified y saturation is out of range (0-1000)." << endl;
                    exit(-1);
                }
                break;
            }

            case 'a': // -a
            {
                string a = string(optarg);
                stringstream ss(a);
                string token;
                vector<int> axis;
                while(getline(ss, token, ','))
                {
                    int num = stoi(token);
                    axis.push_back(num);
                }
                this->opt_axes = axis;
                axis_selected = true;
                break;
            }

            case 'd': // -d
            {
                string a = string(optarg);
                stringstream ss(a);
                string token;
                vector<int> dev;
                while(getline(ss, token, ','))
                {
                    int num = stoi(token);
                    dev.push_back(num);
                }
                this->opt_devices = dev;
                device_selected = true;
                break;
            }

            case 'c':
            {
                setting_specified = true;
                require_axis = true;
                value = stoi(string(optarg));
                if (value >= 0 && value <= 1000)
                {
                    this->opt_curve = value;
                    break;
                }
                else
                {
                    cerr << "Specified curvature is out of range (0-1000)." << endl;
                    exit(-1);
                }
                break;
            }
            case '?':
                break;
            default:
                break;
        }

        if (setting_specified && !device_selected)
        {
            cerr << "A device must be selected, run without arguments for device list." << endl;
            exit(-1);
        }
        if (require_axis && !axis_selected)
        {
            cerr << "Axis must be specified for given options, use --get to list axes." << endl;
            exit(-1);
        }
    }
}
