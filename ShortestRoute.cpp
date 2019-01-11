// Term Project, The "ShortestRoute" Program
// Programmer: Fernando Ian Patricio
// Editor(s) used: Code:Blocks
// Compiler(s) used: GNU GCC Compiler

#include <iostream>
using std::cout;
using std::endl;
using std::ostream;

#include <string>
using std::string;

#include <vector>
using std::vector;

class Route; // class Route prototype
class ShortestRoute;

class Leg
{
  friend class Route; // Friend class Route of class Leg
  friend class ShortestRoute;

  friend void outputLeg( ostream&, const Leg& ); // Friend function of class Leg
  friend ostream& operator <<( ostream&, const Route& ); // friend overloaded operator <<

  private:
    const char* const startingCity; // name of the startingCity
    const char* const endingCity; // name of the endingCity
    const double distance; // distance between the startingCity and endingCity  in miles

  public:
    Leg( const char* const, const char* const, const double ); // Constructor to initialize data members
}; // end class Leg

// Leg constructor to initialize the private data members of class Leg using initializer list
Leg::Leg( const char* const startCity , const char* const endCity, const double dist )
  : startingCity( startCity ), endingCity( endCity ), distance( dist  ) {} // end Leg constructor

// Friend Function to output Leg object data
void outputLeg( ostream& out, const Leg& leg)
{
  out << leg.startingCity << " to " << leg.endingCity << ", " << leg.distance << " miles.\n";
} // end friend function outputLeg of class Leg

class Route
{
  friend void outputRoute(ostream&, const Route&); // Friend function of class Route

  private:
    const Leg** const legs; // const Leg ** const data member for saving a dynamically-sized array of Leg*'s
    const int sizeOfLegs;   // const int data member to save the size of the Leg* array
    const double distanceOfLegs; // const double data member to store the distance of the Route, computed as the sum of the distances of its Legs.

  public:
    Route( const Route& ); // Copy constructor to copy a route
    Route( const Leg& ); // First constructor passing a Leg
    Route( const Route&, const Leg& ); // Second constructor passing a Route and a Leg
    bool operator <( const Route& r ) { return distanceOfLegs < r.distanceOfLegs; } // overloaded operator less-than
    Route& operator=(const Route&); // overloaded operator = to use for vector::push_back()

    friend ostream& operator <<( ostream&, const Route& ); // friend overloaded operator <<
    ~Route(); // Destructor
};

// Route copy constructor to copy a route
Route::Route( const Route& r )
  : legs ( new const Leg*[r.sizeOfLegs] ),  // allocate dynamically sized array of pointers legs that points to Leg objects
    sizeOfLegs( r.sizeOfLegs ), // initialize sizeOfLegs by the the number of legs in the route
    distanceOfLegs ( r.distanceOfLegs ) // initialize distanceOfLegs by the distance of the route
{
  // for loop to store the addresses of the Route objects to the array of pointers legs
  for( int i = 0; i < r.sizeOfLegs; i ++ )
    legs[i] = r.legs[i];
} // end Route copy constructor

// First constructor to create a simple route consisting of only one leg.
Route::Route( const Leg& l )
  : legs( new const Leg*[1] ), // allocate dynamically sized array of pointers legs that point to Leg objects
    sizeOfLegs( 1 ), // initialize sizeOfLegs by 1 because there is only 1 leg
    distanceOfLegs( l.distance ) // initialize distanceOfLegs by the distance of the Leg
{
  legs[0] = &l; // store the address of the Leg object to an array of pointer legs that points to a Leg object
} // end class Route first constructor

// Second constructor to create a new route by adding a leg to the end of an
// existing route whose starting city should match the ending city of the Route object
Route::Route( const Route& r, const Leg& l )
  : legs( new const Leg*[r.sizeOfLegs + 1] ), // initialize legs by allocating dynamically sized array of pointers legs that point to Leg objects
    sizeOfLegs( r.sizeOfLegs + 1 ), // initialize sizeOfLegs by [the number of legs in the route + 1(the new leg)]
    distanceOfLegs( r.distanceOfLegs + l.distance ) // initialize distanceOfLegs by (the distance of the route + the distance of the new Leg)
{
  // for loop to store the addresses of Route objects to the array of pointers legs
  for( int j = 0; j < r.sizeOfLegs; j++ )
    legs[j] = r.legs[j];

  // Add Leg to the Route
  legs[r.sizeOfLegs] = &l; // add the address of the new Leg object to the array of pointer legs
} // end class Route second constructor

// Route destructor deallocates dynamically allocated memory
Route::~Route()
{
  delete [] legs;
} // end Route destructor

// overloaded operator <<
ostream& operator <<( ostream& out, const Route& r )
{
  out << ( r.legs[0] )->startingCity; // print starting city

  // traverse the route's Leg pointer array in order to list only the ending city of each leg
  for ( int k = 0; k < r.sizeOfLegs; k ++ )
    out << " to " << ( r.legs[k] )->endingCity; // print ending city

  out << ", " << r.distanceOfLegs << " miles.\n"; // print total distance of the route

  return out;
} // end friend function outputRoute of class Leg and class Route

Route& Route::operator=(const Route& r)
{
  if (this == &r) return *this;

  // legs = new const Leg*[r.sizeOfLegs + 1];
  const_cast<const Leg**&>(legs) = ( new const Leg*[r.sizeOfLegs] );

  //sizeOfLegs = r.sizeOfLegs;
  const_cast<int&>(sizeOfLegs) = r.sizeOfLegs;

  // distanceOfLegs = r.distanceOfLegs;
  const_cast<double&>(distanceOfLegs) = r.distanceOfLegs;

  // for loop to store the addresses of the Route objects to the array of pointers legs
  for( int i = 0; i < r.sizeOfLegs; i ++ )
    legs[i] = r.legs[i];

  return *this;
}

class ShortestRoute
{
  private:
    static const int legsSize;
    static const Leg legs[];

  public:
    static Route getAnyRoute( const string, const string );
    static Route getShortestRoute( const string, const string );
};
const int ShortestRoute::legsSize = 41;

// database
const Leg ShortestRoute::legs[] =
{
  Leg("San Francisco", "Sacramento", 88),
  Leg("San Francisco", "Fresno", 188),
  Leg("San Francisco", "Bakersfield", 283),
  Leg("Sacramento", "Boise", 552),
  Leg("Sacramento", "Reno", 132),
  Leg("Fresno", "Reno", 300),
  Leg("Fresno", "Las Vegas", 395),
  Leg("Bakersfield", "Las Vegas", 285),
  Leg("Bakersfield", "Tucson", 599),
  Leg("Reno", "Boise", 421),
  Leg("Reno", "Salt Lake City", 518),
  Leg("Las Vegas", "Salt Lake City", 420),
  Leg("Las Vegas", "Tucson", 413),
  Leg("Boise", "Sheridan", 742),
  Leg("Boise", "Salt Lake City", 340),
  Leg("Salt Lake City", "Sheridan", 554),
  Leg("Salt Lake City", "Cheyyene", 440),
  Leg("Salt Lake City", "Santa Fe", 626),
  Leg("Tucson", "Santa Fe", 495),
  Leg("Tucson", "Roswell", 464),
  Leg("Sheridan", "Pierre", 390),
  Leg("Sheridan", "Cheyyene", 326),
  Leg("Cheyyene", "Pierre", 425),
  Leg("Cheyyene", "Omaha", 494),
  Leg("Santa Fe", "Wichita", 567),
  Leg("Santa Fe", "Roswell", 192),
  Leg("Roswell", "Wichita", 568),
  Leg("Roswell", "Oklahoma City", 469),
  Leg("Pierre", "Columbus", 1145),
  Leg("Pierre", "Omaha", 405),
  Leg("Omaha", "Indianapolis", 615),
  Leg("Omaha", "Charleston", 922),
  Leg("Wichita", "Indianapolis", 676),
  Leg("Wichita", "Oklahoma City", 161),
  Leg("Oklahoma City", "Columbus", 916),
  Leg("Oklahoma City", "Charleston", 1003),
  Leg("Indianapolis", "Columbus", 175),
  Leg("Indianapolis", "Charleston", 311),
  Leg("Columbus", "New York City", 536),
  Leg("Charleston", "New York City", 532),
  Leg("San Francisco", "New York City", 21000)
};

Route ShortestRoute::getAnyRoute( const string from, const string to )
{
  for (int i = 0; i < legsSize; i++)
  {
    if( legs[i].endingCity == to )
    {
      if ( legs[i].startingCity == from )
      {
        Route r( legs[i] );
        return r;
      }

      else
      {
        Route r = getAnyRoute( from, legs[i].startingCity );
        Route r2( r, legs[i] );
        return r2;
      }
    }
  }

  throw "No route can be found from the from-city to the to-city!";
}

Route ShortestRoute::getShortestRoute( const string from, const string to )
{
  vector<Route> routes;

  for (int j = 0; j < legsSize; j++)
  {
    if( legs[j].endingCity == to )
    {
      if ( legs[j].startingCity == from )
      {
        Route r( legs[j] );
        routes.push_back(r);
      }

      else
      {
        Route r = getShortestRoute( from, legs[j].startingCity );
        Route r2( r, legs[j] );
        routes.push_back(r2);
      }
    }
  }

  if ( routes.empty() )
    throw "No route found";

  if ( !routes.empty() )
  {
    int shortestRouteIndex = 0;

    for(unsigned int i = 1; i < routes.size(); i++)
    {
      if( routes[i] < routes[shortestRouteIndex] )
        shortestRouteIndex = i;
    }

    return routes[shortestRouteIndex];
  }
}

int main()
{
  // print my name and this assignment's title
  cout << "----------------------------------------------------------------------------------------\n";
  cout << "Term Project, The \"ShortestRoute\" Program \n";
  cout << "Programmer: Fernando Ian Patricio\n";
  cout << "Editor(s) used: Code::Blocks\n";
  cout << "Compiler(s) used: GNU GCC Compiler\n";
  cout << "File: " << __FILE__ << endl;
  cout << "Compiled: " << __DATE__ << " at " << __TIME__ << endl;
  cout << "----------------------------------------------------------------------------------------\n\n";

  cout << "Calling ShortestRoute's getAnyRoute...\n";
  cout << ShortestRoute::getAnyRoute("San Francisco", "New York City");
  cout << endl;

  cout << "Calling ShortestRoute's shortestRoute...\n";
  cout << ShortestRoute::getShortestRoute("San Francisco", "New York City");
} // end int main






