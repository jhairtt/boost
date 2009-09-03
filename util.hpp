// Copyright (C) 2009 by Jhair Tocancipa Triana

#ifndef UTIL_HPP
#define UTIL_HPP

#include <boost/math/special_functions/binomial.hpp>

namespace Util
{
  typedef std::set<int> SetInt;

  template <typename T> struct PrintFunctor
  {
    PrintFunctor(std::ostream& out) : os(out) {}
    void operator() (T x) { os << x << ' '; }

    std::ostream& os;
  };

  inline std::string trimString (std::string const & sString)
  {
    std::string::size_type start = 0, end = sString.length () - 1;

    while (start <= end && sString [start] == ' ')
    {
      start++;
    }
    while (sString [end] == ' ')
    {
      end--;
    }
    return sString.substr (start, end - start + 1);
  }

  inline std::vector <std::string> splitString (std::string const& sString, 
                                                char delimiter = ',')
  {
    std::vector <std::string> results;

    std::string::size_type currentPos = sString.find_first_not_of ("\t ");
    while (currentPos < sString.length ())
    {
      // get the string until the delimiter
      std::string::size_type nextPos = sString.find_first_of (delimiter, 
                                                              currentPos);
      std::string sCurrent = sString.substr (currentPos, nextPos - currentPos);

      // append to the list
      results.push_back (Util::trimString (sCurrent));

      if (nextPos != std::string::npos)
        currentPos = nextPos + 1;
      else
        currentPos = nextPos;
    }
    return results;
  }

  inline int stringToInt (std::string const &sString)
  {
    // use a string stream to read from
    std::istringstream is (sString);

    int result = 0;
    is >> result;

    return result;
  }

  inline std::string intToString (int x)
  {
    // use a string stream to write on
    std::ostringstream os;

    os << x;

    return os.str ();
  }

  inline int xi_function(int n)
  {
    for (int i = 0; i <= n; i++)
    {
      if (boost::math::binomial_coefficient<double>(i, floor(i / 2)) >= n)
      {
        return i;
      }
    }
  }

  void ksub_next(int n, int k, int a[], bool *more)
  {
    int j;
    static int m = 0;
    static int m2 = 0;

    if ( k < 0 || n < k )
    {
      std::cout << "\n";
      std::cout << "KSUB_NEXT - Fatal error!\n";
      std::cout << "N = " << n << "\n";
      std::cout << "K = " << k << "\n";
      std::cout << "but 0 <= K <= N is required!\n";
      exit ( 1 );
    }

    if ( !( *more ) )
    {
      m2 = 0;
      m = k;
    }
    else
    {
      if ( m2 < n-m )
      {
        m = 0;
      }
      m = m + 1;
      m2 = a[k-m];
    }

    for ( j = 1; j <= m; j++ )
    {
      a[k+j-m-1] = m2 + j;
    }

    *more = ( a[0] != (n-k+1) );

    return;
  }

  std::map<int, SetInt> k_choose_n(std::set<int> s, int k)
  {
    int n = s.size();
    bool more = false;
    int rank = 0;

    std::cout << "main set:";  
    std::for_each(s.begin(), s.end(), PrintFunctor<int>(std::cout));
    std::cout << std::endl;

    std::map<int, SetInt> result;

    std::vector<int> a(s.begin(), s.end());

    while (true)
    {
      ksub_next(n, k, &a[0], &more);

      rank = rank + 1;

      // Put the permutations in the map
      std::set<int> permutation;

      std::cout << std::setw(4) << rank << "    ";
      for (int i = 0; i < k; i++ )
      {
        std::cout << std::setw(4) << a[i];

        permutation.insert(a[i]);
      }
      std::cout << "\n";

      result[rank] = permutation;

      if (!more )
      {
        break;
      }
    }

    return result;
  }
};

#endif // UTIL_HPP
