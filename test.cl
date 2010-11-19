class Main inherits IO {
    x : Object <- new Object;
    y : String <- x.type_name();
    main() : SELF_TYPE {
{
        out_string("The type name is... :");
        out_string(y);
	    --out_string(y.substr(4,1));
}
    };
};
