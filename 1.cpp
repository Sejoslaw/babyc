	/*expression EQ expression == euqal_equal_new
        {
            Syntax *right = stack_pop(syntax_stack);
            Syntax *left = stack_pop(syntax_stack);
            stack_push(syntax_stack, euqal_equal_new(left, right));
        }
        |
	expression NE expression != not_equal_new
        {
            Syntax *right = stack_pop(syntax_stack);
            Syntax *left = stack_pop(syntax_stack);
            stack_push(syntax_stack, not_equal_new(left, right));
        }
        |
	expression AND expression  && and_judge 
        {
            Syntax *right = stack_pop(syntax_stack);
            Syntax *left = stack_pop(syntax_stack);
            stack_push(syntax_stack, and_judge(left, right));
        }
        |
	expression OR expression  || or_judge 
        {
            Syntax *right = stack_pop(syntax_stack);
            Syntax *left = stack_pop(syntax_stack);
            stack_push(syntax_stack, or_judge(left, right));
        }
        |*/



	/*expression '/' expression 除法 division_new
        {
            Syntax *right = stack_pop(syntax_stack);
            Syntax *left = stack_pop(syntax_stack);
            stack_push(syntax_stack, division_new(left, right));
        }
        |*/
