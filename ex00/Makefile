# **************************************************************************** #
#                                Makefile (C++)                                #
# **************************************************************************** #

NAME     := convert

CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -g3

OBJDIR   := obj

SRCS     := main.cpp \
            ScalarConverter.cpp

OBJS     := $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
DEPS     := $(OBJS:.o=.d)

# **************************************************************************** #
#                                   Rules                                      #
# **************************************************************************** #

all: $(NAME)
	@echo "✅ $(NAME) built successfully!"

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@
	@echo "🚀 Linked: $(NAME)"

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
	@echo "🔧 Compiled: $<"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR)
	@echo "🧹 Cleaned objects."

fclean: clean
	@rm -f $(NAME)
	@echo "🗑️  Removed binary."

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
