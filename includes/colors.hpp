#ifndef __COLORS_HPP__
# define __COLORS_HPP__

# define BOLD        "\033[1m"
# define DIM         "\033[2m"
# define UNDERLINED  "\033[4m"
# define BLINK       "\033[5m"
# define REVERSE     "\033[7m"
# define HIDDEN      "\033[8m"

# define RESET			"\033[0m"
# define RESET_BOLD        "\033[21m"
# define RESET_DIM         "\033[22m"
# define RESET_UNDERLINED  "\033[24m"
# define RESET_BLINK       "\033[25m"
# define RESET_REVERSE     "\033[27m"
# define RESET_HIDDEN      "\033[28m"

# define BLACK    "\033[30m"
# define RED      "\033[31m"
# define GREEN    "\033[32m"
# define YELLOW   "\033[33m"
# define BLUE     "\033[34m"
# define MAGENTA  "\033[35m"
# define CYAN     "\033[36m"
# define LGRAY    "\033[37m"
# define DGRAY    "\033[90m"
# define LRED     "\033[91m"
# define LGREEN   "\033[92m"
# define LYELLOW  "\033[93m"
# define LBLUE    "\033[94m"
# define LMAGENTA "\033[95m"
# define LCYAN    "\033[96m"
# define WHITE    "\033[97m"

# define BG_DEFAULT  "\033[49m"
# define BG_BLACK    "\033[40m"
# define BG_RED      "\033[41m"
# define BG_GREEN    "\033[42m"
# define BG_YELLOW   "\033[43m"
# define BG_BLUE     "\033[44m"
# define BG_MAGENTA  "\033[45m"
# define BG_CYAN     "\033[46m"
# define BG_LGRAY    "\033[47m"
# define BG_DGRAY    "\033[100m"
# define BG_LRED     "\033[101m"
# define BG_LGREEN   "\033[102m"
# define BG_LYELLOW  "\033[103m"
# define BG_LBLUE    "\033[104m"
# define BG_LMAGENTA "\033[105m"
# define BG_LCYAN    "\033[106m"
# define BG_WHITE    "\033[107m"

# define INFO(func)	BOLD""BLUE"[INFO] "CYAN""func" "RESET
# define ERROR(func) BOLD""RED"[ERROR] "LRED""func" "RESET

#endif
