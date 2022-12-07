/// <reference types="@altv/types-shared"/>
/// <reference types="@altv/types-client"/>
/// <reference types="@altv/types-server"/>
/// <reference types="@altv/types-natives"/>

declare const alt: typeof import("alt-shared") & typeof import("alt-client") & typeof import("alt-server");
declare const native: typeof import("natives");
