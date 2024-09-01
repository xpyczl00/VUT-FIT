#include <simlib.h>
#include <iostream>
#include <random>
#include <string>
#include <cstdlib>
// structure for arguments
struct Options
{
    int delivery_time;
    int delivery_size;
    int presser_size;
    int barrel_size;
    int simulation_length;
    int garage_size;
    bool show_help;
};
// printing Usage
void printUsage(const std::string &programName)
{
    std::cout << "Usage: " << programName << " [options]\n"
              << "Options:\n"
              << "  -dt, --delivery_time    Set delivery time (integer)\n"
              << "  -ds, --delivery_size    Set delivery size (integer)\n"
              << "  -ps, --presser_size     Set presser capacity (integer)\n"
              << "  -bs, --barrel_size      Set barrel capacity (integer)\n"
              << "  -sl, --simulation_length Set simulation length in minutes (integer)\n"
              << "  -gs, --garage_size      Set garage size (integer)\n"
              << "  -h   --help             Display this help message\n";
}
// parsing arguments
Options parseArguments(int argc, char *argv[])
{
    Options options = {0, 0, 0, 0, 0, 0, false};

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-dt" || arg == "--delivery_time")
        {
            if (++i < argc)
            {
                options.delivery_time = std::stoi(argv[i]);
            }
            else
            {
                std::cerr << "Error: Missing value for delivery_time option.\n";
                std::exit(1);
            }
        }
        else if (arg == "-ds" || arg == "--delivery_size")
        {
            if (++i < argc)
            {
                options.delivery_size = std::stoi(argv[i]);
            }
            else
            {
                std::cerr << "Error: Missing value for delivery_size option.\n";
                std::exit(1);
            }
        }
        else if (arg == "-gs" || arg == "--garage_size")
        {
            if (++i < argc)
            {
                options.garage_size = std::stoi(argv[i]);
            }
            else
            {
                std::cerr << "Error: Missing value for garage_size option.\n";
                std::exit(1);
            }
        }

        else if (arg == "-ps" || arg == "--presser_size")
        {
            if (++i < argc)
            {
                options.presser_size = std::stoi(argv[i]);
            }
            else
            {
                std::cerr << "Error: Missing value for presser_size option.\n";
                std::exit(1);
            }
        }

        else if (arg == "-bs" || arg == "--barrel_size")
        {
            if (++i < argc)
            {
                options.barrel_size = std::stoi(argv[i]);
            }
            else
            {
                std::cerr << "Error: Missing value for barrel_size option.\n";
                std::exit(1);
            }
        }
        else if (arg == "-sl" || arg == "--simulation_length")
        {
            if (++i < argc)
            {
                options.simulation_length = std::stoi(argv[i]);
            }
            else
            {
                std::cerr << "Error: Missing value for simulation_length option.\n";
                std::exit(1);
            }
        }
        else if (arg == "-h" || arg == "--help")
        {
            options.show_help = true;
            return options;
        }

        else
        {
            std::cerr << "Error: Unknown option: " << arg << "\n";
            std::exit(1);
        }
    }

    return options;
}

// constants
long GARAGE_SIZE;                                    // kg
long GRAPE_DELIVERY;                                 // kg per day
double DELIVERY_INTERVAL;                            //  hours
long PRESSER_CAPACITY;                               // kg
const double PRESSING_TIME = 30;                     // min
const long PRESSER_COUNT = 3;                        // pcs
long BARREL_SIZE;                                    // l
const long BARREL_COUNT = 5;                         // pcs
long BARREL_SIZE_TOTAL = BARREL_SIZE * BARREL_COUNT; // l
long PRESSER_OUTPUT;                                 // kg

// time counters
double time_waiting_for_presses = 0;
double time_waiting_for_delivery = 0;
double last_press = 0;

double last_delivery = 0;
double time_waiting_for_barrels = 0;
double last_free_barrel = 0;
double total_time_spent_winemaking = 0;
double total_time_pressing = 0;
// Define the facilities
Facility Presser[3];
Facility Barrel[5];

// Define the stores
Store BARRELS_CAPACITY("Barrel Storage", BARREL_SIZE_TOTAL);
Store GARAGE("Garage", GARAGE_SIZE);

// additional stats
long long thrown_away = 0;
long long delivered_total = 0;
long long presser_counter = 0;
long long barrel_counter =0;
long long wine_produced = 0;
long long presser_production_mean;
long long barrel_production_mean;


// variables for malfunction
bool presser_malfunction = false;
int random_number = 0;
double total_time_malfunctioning = 0;

// Function to generate a random number between 0.80 and 0.85
double generateRandomNumber()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.80, 0.85);

    return dis(gen);
}
// Malfunction Generator
class Presser_Malfunction : public Event
{
    void Behavior() override
    {

        if (Random() < 0.1)
        {
            presser_malfunction = true;
        }
        else
        {
            presser_malfunction = false;
        }

        (new Presser_Malfunction)->Activate(Time + Normal(1440,100));
    }
};

// Process of WINE_MAKING
class WINE_MAKING : public Process
{
    void Behavior() override
    {
        for (size_t i = 0; i < BARREL_COUNT; i++)
        {

            if (BARRELS_CAPACITY.Used() >= BARREL_SIZE && !Barrel[i].Busy())
            {
                barrel_counter++;
                double a = Time;
                Leave(BARRELS_CAPACITY, BARREL_SIZE);
                if (last_free_barrel != 0)
                {
                    Print("Time waiting for free barrel : %f \n", Time - last_free_barrel);
                    time_waiting_for_barrels += (Time - last_free_barrel);
                    last_free_barrel = 0;
                }
                Seize(Barrel[i]);

                Print("Barrel %d Spinning has begun\n", i);
                Wait(Normal(120, 30));
                Print("Barrel %d Spinning has ended\n", i);
                Print("Barrel %d Fermentation has begun\n", i);
                
                Wait(Normal(60 * 24 * 14, 5 * 60 * 24));
                
                Print("Barrel %d Fermentation has ended\n", i);
                Print("Barrel %d second spinning has begun\n", i);
                Wait(Normal(120, 30));
                Print("Barrel %d Second Spinning has ended\n", i);
                Print("Barrel %d Aging has begun\n", i);
                Wait(Normal(60 * 24 * 75, 60 * 24 * 15));
                Print("Barrel %d aging has ended\n", i);

                Release(Barrel[i]);

                // testing the wine, chance of bad batch
                if (Random() < 0.01)
                {
                    thrown_away += BARREL_SIZE;
                }
                else
                {

                    wine_produced = wine_produced + BARREL_SIZE;
                }
                total_time_spent_winemaking = total_time_spent_winemaking + (Time - a);
                (new WINE_MAKING)->Activate();
            }
        }
    }
};

class PressingGrapes : public Process
{
    void Behavior() override
    {
        // Wait until there are enough grapes for the mixer
        if (GARAGE.Used() >= PRESSER_CAPACITY)
        {

            if (presser_malfunction)
            {
                random_number = rand() % (int)(PRESSER_COUNT - 1);
            }

            for (size_t i = 0; i < PRESSER_COUNT; i++)
            {

                if (!Presser[i].Busy() && GARAGE.Used() >= PRESSER_CAPACITY && BARRELS_CAPACITY.Free() >= PRESSER_OUTPUT)
                {
                     double o = Time;
                    if (presser_malfunction && i == random_number)
                    {
                        presser_malfunction = false;
                        double t = Time;
                        Seize(Presser[i]);
                        Leave(GARAGE, PRESSER_CAPACITY);
                        Wait(Normal(45, 15));
                        thrown_away += PRESSER_OUTPUT;
                        Release(Presser[i]);
                        total_time_malfunctioning += (Time - t);
                    }

                    else
                    {
                        last_press = Time;
                        presser_counter++;
                        Seize(Presser[i]);
                        double time = Time;

                        Print("Presser %d started pressing\n", i);
                        Leave(GARAGE, PRESSER_CAPACITY);

                        Wait(Normal(PRESSING_TIME,10));

                        Release(Presser[i]);
                        Print("Presser %d finished pressing, lasted : %g \n", i, Time - time);
                    }
                    total_time_pressing = total_time_pressing + (Time-o);

                    if (BARRELS_CAPACITY.Free() < PRESSER_OUTPUT && BARRELS_CAPACITY.Free() > 0)
                    {
                        long missing = BARREL_SIZE_TOTAL - BARRELS_CAPACITY.Used();
                        Print("%d \n", missing);
                        Enter(BARRELS_CAPACITY, missing);

                        thrown_away += (PRESSER_OUTPUT - missing);
                        last_free_barrel = Time;
                    }
                    else
                    {
                        Enter(BARRELS_CAPACITY, PRESSER_OUTPUT);
                    }
                    Print("Wine in the Barrels : %d \n", BARRELS_CAPACITY.Used());

                    (new PressingGrapes)->Activate();
                }
                if (!Presser[i].Busy() && GARAGE.Used() >= PRESSER_CAPACITY)
                {
                    if (last_press != 0)
                    {
                        Print("Time waiting for press: %f\n", Time - last_press);
                        time_waiting_for_presses += Time - last_press;
                        last_press = 0;
                    }
                }
            }
            for (size_t i = 0; i < BARREL_COUNT; i++)
            {
                if (BARRELS_CAPACITY.Used() >= BARREL_SIZE && !Barrel[i].Busy())
                {

                    (new WINE_MAKING)->Activate();
                }
            }
        }
        else
        {
            Print("Garage is empty : %d Time : %g \n", GARAGE.Used(), Time);

            last_delivery = Time;
        }
    }
};

class Delivery : public Process
{

    void Behavior() override
    {
        while (true)
        {
            double t = Time;
            int Day = (int)(Time / (60 * 24) + 1);
            if (GARAGE.Free() >= GRAPE_DELIVERY)
            {

                if (last_delivery != 0)
                {
                    Print("Time waiting for resources: %f\n  ", Time - last_delivery);
                    time_waiting_for_delivery += (Time - last_delivery);
                    last_delivery = 0;
                }

                Enter(GARAGE, GRAPE_DELIVERY);
                delivered_total += GRAPE_DELIVERY;
                Print("Day %d, Delivered : %lld kg total, remaining storage left : %d \n", Day,
                      delivered_total, GARAGE.Free());
            }
            else
            {

                time_waiting_for_delivery = time_waiting_for_delivery + (Time - t);

                Print("Day %d, Delivered : %lld kg total, remaining storage left : %d \n", Day, delivered_total,
                      GARAGE.Free());
            }
            for (size_t i = 0; i < PRESSER_COUNT; i++)
            {
                if (!Presser[i].Busy() && GARAGE.Used() >= PRESSER_CAPACITY && BARRELS_CAPACITY.Free() >= PRESSER_OUTPUT)
                {
                    (new PressingGrapes)->Activate();
                }
            }
            if (BARRELS_CAPACITY.Used() == BARREL_SIZE_TOTAL)
            {
                Print("Barrels are full \n");
            }

            Activate(Time + DELIVERY_INTERVAL * 60 * 24);
            // Passivate();
        }
    }
};

class GrapeSeason : public Process
{
    Delivery *d;

public:
    GrapeSeason(Delivery *d) : d(d) {}
    void Behavior() override
    {
        while (true)
        {

            d->Activate();
            Print("Start of the harvesting season\n");
            Wait(Exponential(60 * 24 * 30 * 3));

            d->Passivate();
            Print("End of the harvesting season\n");

            Wait(Exponential(60 * 24 * 30 * 9 + 4 * 24 * 60));
        }
    }
};

int main(int argc, char *argv[])
{
    // Set the simulation time unit to minutes
    Options options = parseArguments(argc, argv);

    if (options.show_help)
    {
        printUsage(argv[0]);
        return 0;
    }
    GARAGE_SIZE = options.garage_size;         // kg
    GRAPE_DELIVERY = options.delivery_size;    // kg per day
    DELIVERY_INTERVAL = options.delivery_time; // 24 hours
    PRESSER_CAPACITY = options.presser_size;   // kg

    BARREL_SIZE = options.barrel_size; // l

    BARRELS_CAPACITY.SetCapacity(options.barrel_size * 5);
    GARAGE.SetCapacity(options.garage_size);
    PRESSER_OUTPUT = PRESSER_CAPACITY * generateRandomNumber();
    BARREL_SIZE_TOTAL = BARREL_SIZE * 5;
    // Initialize the simulation
    Init(0, 24 * 60 * options.simulation_length);
    // Create the grape production process
    Delivery *d = new Delivery;
    (new GrapeSeason(d))->Activate();
    (new Presser_Malfunction())->Activate(Time + 1440);
    // Run the simulation
    Run();
    long i = Time / (60 * 24);
   /*  Print("TIME TOTAL days : %d\n", i);

    Print("Thrown away %d \n", thrown_away);
    Print("Left in garage %u \n", GARAGE.Used());
    Print("Pressers used %lld times\n", presser_counter);

    Print("wine_produced : %lld\n", wine_produced);
    Print("total time waiting for presses: %f\n", time_waiting_for_presses);
    Print("total time waiting for resources: %f\n", time_waiting_for_delivery);
    Print("total time waiting for barrels  %f\n", time_waiting_for_barrels);

    Print("total time fermenting  %f\n", total_time_spent_winemaking);
    Print("total time malfunction %f\n", total_time_malfunctioning); */
    
    double presser_efficency = (total_time_pressing/presser_counter)/PRESSER_OUTPUT;
    double barrel_efficency = (total_time_spent_winemaking/barrel_counter)/BARREL_SIZE;
    Print("\n\n================Params==============\n");
    Print("Simulation length : %d days\n",i);
    Print("Delivery interval : %f\nDelivery Size : %d \nGarage size: %d \nPresser capacity : %d \nBarrel size : %d  \nBarrel size total: %d \n",DELIVERY_INTERVAL,GRAPE_DELIVERY,GARAGE_SIZE,PRESSER_CAPACITY,BARREL_SIZE,BARREL_SIZE_TOTAL);
    
    Print("================Efficiency==============\n");
    Print("Yearly efficency mean : %d/%d days\n", wine_produced,i);
    Print("Presser efficency mean : %f/day\n", presser_efficency);
    Print("Barrel efficency mean : %f/day\n", barrel_efficency);

    Print("================Time wasted==============\n");    

    Print("total time waiting for presses: %f\n", time_waiting_for_presses);
    Print("total time waiting for resources: %f\n", time_waiting_for_delivery);
    Print("total time waiting for barrels  %f\n\n\n", time_waiting_for_barrels);

    }
