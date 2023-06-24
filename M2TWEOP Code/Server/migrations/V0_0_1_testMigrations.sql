CREATE TABLE TestDB (
	id int NOT NULL GENERATED ALWAYS AS IDENTITY,
	"text" varchar NULL,
	val int NOT NULL,
	CONSTRAINT testtable_pk PRIMARY KEY (id)
);