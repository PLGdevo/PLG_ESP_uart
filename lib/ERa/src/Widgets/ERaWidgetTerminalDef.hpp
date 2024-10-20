#ifndef INC_ERA_WIDGET_TERMINAL_DEFINE_HPP_
#define INC_ERA_WIDGET_TERMINAL_DEFINE_HPP_

#if !defined(ERA_TERMINAL_OUTPUT_BUFFER_SIZE)
    #define ERA_TERMINAL_OUTPUT_BUFFER_SIZE     ERA_MAX_WRITE_BYTES
#endif

#if !defined(ERA_TERMINAL_INPUT_BUFFER_SIZE)
    #define ERA_TERMINAL_INPUT_BUFFER_SIZE      ERA_MAX_READ_BYTES
#endif

#if defined(ARDUINO) || defined(LINUX) ||   \
    defined(SPARK) || defined(PARTICLE)
    #define ERA_USE_STREAM
#endif

#endif /* INC_ERA_WIDGET_TERMINAL_DEFINE_HPP_ */
