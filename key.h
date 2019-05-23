// https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

class Key
{
public:
  using history_t = uint32_t;
  void setup() 
  {
    pressed = false;
    history = 0;
    ancient_history = 0;
  }

  // axiom: raw_state = 1 if the key is pressed (or bouncing?!), else 0
  void update(const bool& raw_state)
  {
    record(history, raw_state);
    record(ancient_history, !stable_released());
    if (!pressed)
    {
      constexpr uint8_t chatter_thresh = 8;
      if (pop_count() >= chatter_thresh) pressed = true;
    }
    else
    {
      if (very_stable_released()) pressed = false;
    }
  }

  bool stable_released() const
  {
    constexpr history_t mask    = 0b11111111111111111111111111111111;
    constexpr history_t pattern = 0b00000000000000000000000000000000;
    return match(history, mask, pattern);
  }

  bool very_stable_released() const
  {
    constexpr history_t mask    = 0b00000000000000000000000000000011;
    constexpr history_t pattern = 0b00000000000000000000000000000000;
    return match(ancient_history, mask, pattern);
  }

  // http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
  uint8_t pop_count() const
  {
    history_t v = history;
    uint8_t c; // c accumulates the total bits set in v
    for (c = 0; v; c++)
    {
      v &= v - 1; // clear the least significant bit set
    }
    return c;
  }
      
  bool pressed;
  history_t history;
  history_t ancient_history;

private:
  bool match(const history_t& h, const uint32_t& mask, const uint32_t& pattern) const
  {
    return (h & mask) == pattern;
  }

  void record(history_t& h, const bool& state)
  {
    h = h << 1;
    h |= state;
  }
};
