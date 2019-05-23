// https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

class Key
{
public:
  void setup() 
  {
    state = 0;
    history = 0;
  }

  // axiom: raw_state = 1 if the key is pressed (or bouncing?!), else 0
  void update(const int& raw_state)
  {
    history = history << 1;
    history |= raw_state;
  }

  bool pressed()
  {
    constexpr uint32_t mask    = 0b11111100000000001111111111111111;
    constexpr uint32_t pattern = 0b00000000000000001111111111111111;
    return match(mask, pattern);
  }

  bool rising()
  {
    constexpr uint32_t mask    = 0b11111111111000000000011111111111;
    constexpr uint32_t pattern = 0b00000000000000000000011111111111;
    return match(mask, pattern);
  }

  bool stable_pressed()
  {
    constexpr uint32_t mask    = 0b11111111111111111111111111111111;
    constexpr uint32_t pattern = 0b11111111111111111111111111111111;
    return match(mask, pattern);
  }

  bool stable_released()
  {
    constexpr uint32_t mask    = 0b11111111111111111111111111111111;
    constexpr uint32_t pattern = 0b00000000000000000000000000000000;
    return match(mask, pattern);
  }
      
  uint32_t state;
  uint32_t history;

private:
  bool match(const uint32_t& mask, const uint32_t& pattern)
  {
    return (history & mask) == pattern;
  }
};
