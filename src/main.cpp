#include <iostream>

#include "Grammar.hpp"
#include "ItemSets.hpp"

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fputs("error: the number of arguments should be 2.\n", stderr);

    return EXIT_FAILURE;
  }

  auto grammar = parse_grammar(argv[1]);

  auto const print_rule =
    [](Grammar::Rule const &rule, std::vector<std::string> const &lookup) -> void
    {
      std::cout << lookup[rule[0] - MIN_VAR_INDEX] << " : ";

      for (size_t i = 1; i + 1 < rule.size(); i++)
      {
        if (rule[i] >= MIN_VAR_INDEX)
          std::cout << lookup[rule[i] - MIN_VAR_INDEX];
        else
          std::cout << (char)rule[i];
      }
    };

  std::cout << "Grammar:\n";
  for (auto const &elem : grammar.rules)
  {
    std::cout << "  ";
    print_rule(elem, grammar.lookup);
    std::cout << '\n';
  }
  std::cout << '\n';

  auto item_sets = find_item_sets(grammar);

  for (size_t i = 0; i < item_sets.size(); i++)
  {
    std::cout << "State " << i << ":\n  ";
    for (auto const &trans : item_sets[i].actions)
    {
      switch (trans.type)
      {
      case Action::Reduce:
        std::cout << "r(";
        print_rule(*trans.reduce_to, grammar.lookup);
        std::cout << ")";
        break;
      case Action::Shift:
        std::cout << '\'' << (char)trans.source
                  << "\' -> s" << trans.destination;
        break;
      case Action::Goto:
        std::cout << grammar.lookup[trans.source - MIN_VAR_INDEX]
                  << " -> g" << trans.destination;
        break;
      }

      std::cout << "; ";
    }

    std::cout << std::endl;

    for (auto const &item : item_sets[i].itemset)
    {
      std::cout << grammar.lookup[(*item.rule)[0] - MIN_VAR_INDEX] << " : ";

      for (size_t i = 1, end = item.rule->size(); i < end; i++)
      {
        if (i == item.dot)
          std::cout << (i == 1 ? ". " : " . ");

        int const val = (*item.rule)[i];

        if (val == 0)
          std::cout << '\n';
        else if (val >= MIN_VAR_INDEX)
          std::cout << grammar.lookup[val - MIN_VAR_INDEX];
        else
          std::cout << (char)val;
      }
    }

    std::cout << std::endl;
  }
}