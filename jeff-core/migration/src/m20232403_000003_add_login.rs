use sea_orm_migration::prelude::*;

#[derive(DeriveMigrationName)]
pub struct Migration;

#[async_trait::async_trait]
impl MigrationTrait for Migration {
  async fn up(&self, manager: &SchemaManager) -> Result<(), DbErr> {
    manager.alter_table(
      Table::alter()
        .table(User::Table)
        .add_column(
          ColumnDef::new(User::Login)
            .string()
            .not_null()
        )
        .to_owned()
    ).await
  }

  async fn down(&self, manager: &SchemaManager) -> Result<(), DbErr> {
    manager.alter_table(
      Table::alter()
        .table(User::Table)
        .drop_column(User::Login)
        .to_owned()
    ).await
  }
}

#[derive(Iden)]
enum User {
  Table,
  Login,
}
