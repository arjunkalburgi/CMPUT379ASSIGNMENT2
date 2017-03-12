#include "./store.h"

store* newStore(int max) {
	return malloc(max * sizeof(store));
}