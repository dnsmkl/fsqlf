


void tqueue_demo()
{
    printf("Demo tokenqueue\n");
    tqueue_t tq;
    tqueue_init(&tq);

    token_t * t = make_token(TC_CMT, "blabla", 6, 1);
    
    printf("tq.length %d, tqueue_empty %d\n", tq.length, tqueue_empty(&tq));
    tqueue_push_back(&tq, t);
    tqueue_peek_n(&tq, 0);
    printf("tq.length %d, tqueue_empty %d\n", tq.length, tqueue_empty(&tq));
    tqueue_drop_head(&tq);
    printf("tq.length %d, tqueue_empty %d\n", tq.length, tqueue_empty(&tq));
    printf("'tqueue_peek_n(&tq, 0) == token' %d\n", tqueue_peek_n(&tq, 0) == t);
}


int main()
{
    tqueue_demo();
    return 0;
}

